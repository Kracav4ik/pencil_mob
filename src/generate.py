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


class Field:
    def __init__(self, type, name):
        self.type = type
        self.name = name

    def decl_ctor(self):
        return 'const %s& %s' % (self.type, self.name)

    def decl_field(self):
        return '%s %s;' % (self.type, self.name)

    def ctor_init(self):
        return ', %s(%s)' % (self.name, self.name)


class MsgClass:
    def __init__(self, name, fields):
        self.name = name
        self.fields = fields

    def cls_caps(self):
        return ''.join(('_'+c) if c.isupper() else c.upper() for c in self.name)[1:]

    def gen_encode(self):
        return '// TODO: encode'

    def gen_decode(self):
        return '// TODO: decode'

    def write_to(self, f):
        f.write('''struct %(cls)s : MessageBase{
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
    MsgClass('StringMessage', [Field('QString', 'str')]),
    MsgClass('SetClientNameMessage', [Field('QString', 'name')]),
    MsgClass('PathMessage', [
        Field('uint8_t', 'r'),
        Field('uint8_t', 'g'),
        Field('uint8_t', 'b'),
        Field('QVector<QPoint>', 'points')
    ]),
]


def main():
    with open('messages.h', 'w') as f:
        f.write('#pragma once\n\n')
        f.write('\n'.join('#include <%s>' % inc for inc in sys_includes))
        f.write('\n\n')
        f.write('\n'.join('#include "%s"' % inc for inc in includes))

        f.write('\n\n')
        for cls in msg_classes:
            cls.write_to(f)


if __name__ == '__main__':
    main()
