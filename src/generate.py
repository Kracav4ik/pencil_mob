sys_includes = [
    'QString',
    'QByteArray',
    'QPoint',
    'QVector',
]
includes = [
    'enums.h',
    'transport.h',
]


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


tuint8 = Type('uint8_t', True)
tuint32 = Type('uint32_t', True)
tstring = Type('QString')
tpointvector = Type('QVector<QPoint>')


class Field:
    def __init__(self, type, name):
        """
        :type type: Type
        :type name: str
        """
        self.type = type
        self.name = name

    def decl_ctor(self):
        return '%s %s' % (self.type.for_param(), self.name)

    def decl_field(self):
        return '%s %s;' % (self.type.for_field(), self.name)

    def ctor_init(self):
        return ', %s(%s)' % (self.name, self.name)


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
        return '// TODO: encode'

    def gen_decode(self):
        return '// TODO: decode'

    def write_to(self, f):
        f.write('''
struct %(cls)s : MessageBase{
    %(decl_field)s

    %(cls)s(%(decl_ctor)s)
            : MessageBase(%(cls_caps)s)%(ctor_init)s {}

    QByteArray encodeMessage() const override {
        %(encode)s
    }

    %(cls)s(const QByteArray& data)
            : MessageBase(%(cls_caps)s) {
            %(decode)s
    }
};
''' % {
            'cls': self.name,
            'cls_caps': self.cls_caps(),
            'decl_field': '\n    '.join(f.decl_field() for f in self.fields),
            'decl_ctor': ', '.join(f.decl_ctor() for f in self.fields),
            'ctor_init': ''.join(f.ctor_init() for f in self.fields),
            'encode': self.gen_encode(),
            'decode': self.gen_decode()
        })


msg_classes = [
    MsgClass('StringMessage', [Field(tstring, 'str')]),
    MsgClass('SetClientNameMessage', [Field(tstring, 'name')]),
    MsgClass('PathMessage', [
        Field(tuint8, 'r'),
        Field(tuint8, 'g'),
        Field(tuint8, 'b'),
        Field(tpointvector, 'points')
    ]),
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


if __name__ == '__main__':
    main()
