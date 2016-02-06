project('lubar', 'c')

x11 = dependency('x11')
pango = dependency('pangocairo')

exe = executable('lubar', 'main.c',
    dependencies : [
        x11,
        pango
    ])
# test('basic', exe)
