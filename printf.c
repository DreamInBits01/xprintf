#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

/*
when passing a char/short to a varadic function, these types get promoted to an int
when passing a float it becomes a double

purpose -> Uniform size and alignment of data when passed on the stack or registers.

*/
int xprint_char(char const character)
{
    return write(1, &character, 1);
};
int xprint_str(char *string)
{
    int count = 0;
    while (*string)
    {
        count += xprint_char(*string);
        string++;
    };
    return count;
}
int xprint_digit(long digit, int base)
{
    int count = 0;
    char *symbols = "0123456789abcdef";
    if (digit < 0)
    {
        // only for decimal values, hex will be fetched as an unsigned int;
        xprint_char('-');
        //-digit flips its sign to a positive to go into the second case
        return xprint_digit(-digit, 10);
    }
    else if (digit < base)
    {
        // digit is positive & lower than the base;
        xprint_char(symbols[digit]);
    }
    else
    {
        // number bigger than the base & positive
        /*
            digit / base removes the right most digit
            423 => 42 => 4;
            digit % base gets the rightmost digit
            423 => 3 => 2 => 4
        */
        count = xprint_digit(digit / base, base);
        return count + xprint_digit(digit % base, base);
    }
}
int xprint_format(char specifier, va_list ap)
{
    int count = 0;
    if (specifier == 'c')
    {
        count += xprint_char(va_arg(ap, int));
    }
    else if (specifier == 's')
    {
        // char *, to get the pointer to the string
        count += xprint_str(va_arg(ap, char *));
    }
    else if (specifier == 'd')
    {
        count += xprint_digit((long)va_arg(ap, int), 10);
    }
    else if (specifier == 'x')
    {
        count += xprint_digit((long)va_arg(ap, unsigned int), 16);
    }
    else
    {
        return write(1, &specifier, 1);
    };
    return count;
}
int xprintf(const char *format, ...)
{
    va_list ap;
    int count = 0;
    va_start(ap, format);
    while (*format)
    {
        if (*format == '%')
        {
            // to get the format type
            count += xprint_format(*(++format), ap);
        }
        else
        {
            count += write(1, format, 1);
        }
        format++;
    };
    va_end(ap);
    return count;
}
int main()
{
    int count = 0;
    // xprintf("Hello %c", 's');
    count = xprintf("%d\n", 423);
    xprintf("%d", count);
}