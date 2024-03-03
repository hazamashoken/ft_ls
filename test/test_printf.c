#include "ft_printf.h"
#include <stdio.h>
#include <stdint.h>

int    main(void)
{
    printf("r :%d\n", printf("r: %p\n", 0));
    printf("f :%d\n", ft_printf("f: %p\n", 0));
    printf("r: %d\n", printf("r: %c %s %d %i %u %x %X %p %% \n", NULL, NULL, NULL, NULL, NULL, NULL, NULL));
    printf("f: %d\n", ft_printf("f: %c %s %d %i %u %x %X %p %% \n", NULL, NULL, NULL, NULL, NULL, NULL, NULL));
    printf("r: %d\n", printf("r: %c %s %d %i %u %x %X %p %% \n", 'G',"hello", -123, -123, -123, -123, -123, "hello"));
    printf("f: %d\n", ft_printf("f: %c %s %d %i %u %x %X %p %% \n", 'G',"hello", -123, -123, -123, -123, -123, "hello"));
    printf("r: %d\n", printf("r: %c %s %d %i %u %x %X %p %% \n", 'G',"hello", INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX, "hello"));
    printf("f: %d\n", ft_printf("f: %c %s %d %i %u %x %X %p %% \n", 'G',"hello", INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX, "hello"));
    printf("r: %d\n", printf("r: %c %s %d %i %u %x %X %p %% \n", 'G',"hello", INT32_MIN, INT32_MIN, INT32_MIN,INT32_MIN, INT32_MIN, "hello"));
    printf("f: %d\n", ft_printf("f: %c %s %d %i %u %x %X %p %% \n", 'G',"hello", INT32_MIN, INT32_MIN, INT32_MIN,INT32_MIN, INT32_MIN, "hello"));
}