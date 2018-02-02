sys_includes = [
    'QString',
    'QByteArray',
    'QPoint',
    'QVector',
    'QColor',
    'QPolygon',
]
includes = [
    'enums.h',
    'transport.h',
    'Stroke.h',
]


def join_lines(lines):
    return '\n        '.join(lines)


class Type:
    def __init__(self, name, copy=False):
        self.copy = copy
        self.name = name

    def for_param(self):
        if self.copy:
            return self.for_field()
        return 'const %s&' % self.name

    def for_field(self):
        return self.name

    def decode(self, var_name, array_name, use_setter):
        return '/* TODO: decode %s %s here from %s */' % (self.name, var_name, array_name)

    def encode_simple(self, var_name):
        return '/* TODO: encode %s %s here */' % (self.name, var_name)

    def encode(self, simple, var_name):
        result = self.encode_simple(var_name)
        if not simple:
            result = 'array.append(%s);' % result
        return result

    def __str__(self):
        return self.name

    @staticmethod
    def decoded_readonly():
        return False

    @staticmethod
    def setter_name(name):
        return name[0].upper() + name[1:]

    @staticmethod
    def assign(var_name, value, use_setter):
        if use_setter:
            return 'set%s(%s);' % (Type.setter_name(var_name), value)
        else:
            return '%s = %s;' % (var_name, value)


class StringType(Type):
    def __init__(self, name):
        super().__init__(name)

    def decode(self, var_name, array_name, use_setter):
        return self.assign(var_name, '%s(%s)' % (self.name, array_name), use_setter)

    def encode_simple(self, var_name):
        return '%s.toUtf8()' % var_name

    @staticmethod
    def decoded_readonly():
        return True


class StructType(Type):
    def __init__(self, name, fields, getters_setters=True):
        """
        :type fields: list[Field]
        """
        super().__init__(name)
        self.fields = fields
        self.getters_setters = getters_setters

    def decode(self, var_name, array_name, use_setter):
        assert not use_setter, 'use_setter is not supported for struct type %s' % self
        return '\n        '.join('%s.%s' % (var_name, field.decode_lines(array_name, self.getters_setters)) for field in self.fields)

    def encode(self, simple, var_name):
        return '\n        '.join(field.encode_lines(False, var_name, self.getters_setters) for field in self.fields)


class ListType(Type):
    def __init__(self, name, item_type):
        """
        :type item_type: StructType
        """
        super().__init__(name)
        self.item_type = item_type

    def decode(self, var_name, array_name, use_setter):
        assert not use_setter, 'use_setter is not supported for list type %s' % self
        counter = Field(tuint32, '%sCount' % var_name)
        s = '\n        '.join([counter.decl_field(), counter.decode_lines(array_name)])
        subdecode = ''.join('''
            %s %s;
            %s
''' % (f.type.name, f.name, f.decode_lines(array_name)) for f in self.item_type.fields)
        s += '''
        for (int _ = 0; _ < %(counter_name)s; ++_) {%(subdecode)s
            %(var_name)s << %(item_type)s(%(params)s);
        }''' % {
            'var_name': var_name,
            'counter_name': counter.name,
            'item_type': self.item_type.name,
            'subdecode': subdecode,
            'params': ', '.join(f.name for f in self.item_type.fields),
        }
        return s

    def encode(self, simple, var_name):
        counter = Field(tuint32, 'size')
        s = counter.encode_lines(False, var_name)
        item_name = ('%sItem' % var_name).replace('.', '_')
        subencode = self.item_type.encode(False, item_name).replace('\n', '\n    ')
        s += '''
        for (%(item_type)s %(item_name)s : %(var_name)s) {
            %(subencode)s
        }''' % {
            'var_name': var_name,
            'item_type': self.item_type.for_param(),
            'item_name': item_name,
            'subencode': subencode,
        }
        return s


class VaryingIntType(Type):
    def __init__(self, name):
        super().__init__(name, True)

    def decode(self, var_name, array_name, use_setter):
        return self.assign(var_name, 'decodeAndShift(%s)' % array_name, use_setter)

    def encode_simple(self, var_name):
        return 'encode(static_cast<%s>(%s))' % (self.name, var_name)


class FixedIntType(Type):
    def __init__(self, name):
        super().__init__(name, True)

    def decode(self, var_name, array_name, use_setter):
        assignment = self.assign(var_name, 'static_cast<%s>(%s[0])' % (self.name, array_name), use_setter)
        return "%(assignment)s\n        %(array_name)s = %(array_name)s.mid(1);" % {
            'assignment': assignment,
            'array_name': array_name,
        }

    def encode_simple(self, var_name):
        return 'static_cast<%s>(%s)' % (self.name, var_name)


class BoolType(Type):
    def __init__(self, name):
        super().__init__(name, True)

    def decode(self, var_name, array_name, use_setter):
        assignment = self.assign(var_name, "%s[0] != '\\0'" % array_name, use_setter)
        return "%(assignment)s\n        %(array_name)s = %(array_name)s.mid(1);" % {
            'assignment': assignment,
            'array_name': array_name,
        }

    def encode_simple(self, var_name):
        return 'static_cast<char>(%s ? 1 : 0)' % var_name


tuint8 = FixedIntType('uint8_t')
tbool = BoolType('bool')
tuint32 = VaryingIntType('uint32_t')
tstring = StringType('QString')


class Field:
    def __init__(self, field_type, field_name):
        """
        :type field_type: Type
        :type field_name: str
        """
        self.type = field_type
        self.name = field_name

    def __str__(self):
        return '%s %s' % (self.type, self.name)

    def decl_ctor(self):
        return '%s %s' % (self.type.for_param(), self.name)

    def decl_field(self):
        return '%s %s;' % (self.type.for_field(), self.name)

    def ctor_init(self):
        return ', %s(%s)' % (self.name, self.name)

    def encode_lines(self, simple, qualifier=None, use_getter=True):
        if qualifier:
            name = '%s.%s' % (qualifier, self.name)
            if use_getter:
                name += '()'
        else:
            name = self.name

        return self.type.encode(simple, name)

    def decode_lines(self, array_name, use_setter=False):
        return self.type.decode(self.name, array_name, use_setter)


tpoint = StructType('QPoint', [Field(tuint32, 'x'), Field(tuint32, 'y')])
tpointvector = ListType('QVector<QPoint>', tpoint)
tcolor = StructType('QColor', [Field(tuint8, 'red'), Field(tuint8, 'green'), Field(tuint8, 'blue')])
tpolygon = ListType('QPolygon', tpoint)
trtroke = StructType('Stroke', [Field(tcolor, 'color'), Field(tbool, 'isEraser'), Field(tpolygon, 'polygon')], False)
tpointstroke = ListType('QVector<Stroke>', trtroke)


class MsgClass:
    def __init__(self, name, fields):
        """
        :type name: str
        :type fields: list[Field]
        """
        self.name = name
        self.fields = fields

    def cls_caps(self):
        return ''.join(('_'+c) if c.isupper() else c.upper() for c in self.name)[1:]

    def gen_encode(self):
        snippets = []
        if len(self.fields) > 1:
            snippets.append('QByteArray array;')
            for f in self.fields:
                snippets.append(f.encode_lines(False))
            snippets.append('return array;')
        elif len(self.fields) == 1:
            snippets.append('return %s;' % self.fields[0].encode_lines(True))
        else:
            snippets.append('return {};')
        return '\n\n'.join('        ' + s for s in snippets)

    def gen_decode(self):
        snippets = []
        if len(self.fields) > 1 or not self.fields[0].type.decoded_readonly():
            snippets.append('QByteArray m = data;')
            array_name = 'm'
        elif len(self.fields) == 1:
            array_name = 'data'
        for f in self.fields:
            snippets.append(f.decode_lines(array_name))
        return '\n\n'.join('        ' + s for s in snippets)

    def write_to(self, f):
        if self.fields:
            decl_field = '''
    %s
''' % '\n    '.join(f.decl_field() for f in self.fields)
            decode_body = '''
%s
    ''' % self.gen_decode()
        else:
            decl_field = ''
            decode_body = ''
        f.write('''
struct %(cls)s : MessageBase{%(decl_field)s
    %(explicit_ctor)s%(cls)s(%(decl_ctor)s)
            : MessageBase(%(cls_caps)s)%(ctor_init)s {}

    QByteArray encodeMessageBody() const override {
%(encode)s
    }

    explicit %(cls)s(const QByteArray& data)
            : MessageBase(%(cls_caps)s) {%(decode)s}
};
''' % {
            'cls': self.name,
            'cls_caps': self.cls_caps(),
            'decl_field': decl_field,
            'decl_ctor': ', '.join(f.decl_ctor() for f in self.fields),
            'explicit_ctor': 'explicit ' if len(self.fields) == 1 else '',
            'ctor_init': ''.join(f.ctor_init() for f in self.fields),
            'encode': self.gen_encode(),
            'decode': decode_body,
        })


msg_classes = [
    MsgClass('StringMessage', [Field(tstring, 'str')]),
    MsgClass('SetClientInfoMessage', [Field(tstring, 'name')]),
    MsgClass('PathMessage', [
        Field(tcolor, 'color'),
        Field(tuint32, 'layerId'),
        Field(tbool, 'isEraser'),
        Field(tpointvector, 'points')
    ]),
    MsgClass('AddNewLayerMessage', [Field(tuint32, 'layerId'), Field(tstring, 'layerName')]),
    MsgClass('RenameLayerMessage', [Field(tuint32, 'layerId'), Field(tstring, 'layerName')]),
    MsgClass('MoveLayerMessage', [Field(tuint32, 'layerId'), Field(tuint32, 'newPos')]),
    MsgClass('RemoveLayerMessage', [Field(tuint32, 'layerId')]),
    MsgClass('RemoveLastStrokeMessage', [Field(tuint32, 'layerId')]),
    MsgClass('CopyLayerMessage', [Field(tuint32, 'fromUserId'), Field(tuint32, 'fromLayerId'), Field(tuint32, 'toLayerId')]),
    MsgClass('LayerContentsMessage', [Field(tuint32, 'layerId'), Field(tpointstroke, 'strokes'), Field(tstring, 'layerName')]),
]


def main():
    with open('messages.h', 'w') as f:
        f.write('#pragma once\n\n')
        f.write('\n'.join('#include <%s>' % inc for inc in sys_includes))
        f.write('\n\n')
        f.write('\n'.join('#include "%s"' % inc for inc in includes))

        f.write('\n')
        for cls in msg_classes:
            cls.write_to(f)

    with open('enums.h', 'w') as f:
        names = [msg.cls_caps() for msg in msg_classes]
        names[0] += ' = 1'
        f.write('''#pragma once
#include <cstdint>

enum MessageType {
    %s,
};

const char* getMessageTypeString(uint32_t type);
''' % ',\n    '.join(names))

    with open('enums.cpp', 'w') as f:
        f.write('''#include "enums.h"

const char* getMessageTypeString(uint32_t type) {
    switch (type) {
        %s
    }
    return "<invalid>";
}
''' % '\n        '.join('case %s: return "%s";' % (msg.cls_caps(), msg.name) for msg in msg_classes))

    with open('MessageHandler.h', 'w') as f:
        f.write('''#pragma once

#include <cstdint>

class QByteArray;

%(cls_fwd)s

class MessageHandler {
public:
    void handle(uint32_t user, uint32_t messageType, const QByteArray& message);

protected:
    %(virt)s

    virtual ~MessageHandler() = default;
};
''' % {
            'cls_fwd': '\n'.join('class %s;' % msg.name for msg in msg_classes),
            'virt': '\n    '.join('virtual void handle%(name)s(uint32_t user, const %(name)s& m) = 0;' % {'name': msg.name} for msg in msg_classes),
        })

    with open('MessageHandler.cpp', 'w') as f:
        f.write('''#include "MessageHandler.h"

#include "enums.h"
#include "messages.h"

void MessageHandler::handle(uint32_t user, uint32_t messageType, const QByteArray& message) {
    switch (messageType) {%s
    }
}
''' % ''.join('''
        case %(NAME)s: {
            %(name)s msg(message);
            handle%(name)s(user, msg);
            break;
        }''' % {'name': msg.name, 'NAME': msg.cls_caps()} for msg in msg_classes))

if __name__ == '__main__':
    main()
