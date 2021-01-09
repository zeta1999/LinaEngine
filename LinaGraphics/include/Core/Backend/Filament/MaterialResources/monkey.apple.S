    .global _MONKEY_MONKEY_OFFSET;
    .global _MONKEY_MONKEY_SIZE;

    .global _MONKEY_PACKAGE
    .section __TEXT,__const
_MONKEY_PACKAGE:
    .incbin "monkey.bin"
_MONKEY_MONKEY_OFFSET:
    .int 0
_MONKEY_MONKEY_SIZE:
    .int 521476

