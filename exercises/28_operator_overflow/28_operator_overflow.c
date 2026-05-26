#include <stdio.h>
#include <limits.h>

#define CHECK_OVERFLOW(carry) \
    carry ? "Overflow" : "Not Overflow"

int check_add_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        "add %2, %1\n\t"      // a + b, 结果存入 %1
        "setc %0"             // 如果进位标志(CF)被设置,则 carry = 1
        : "=r" (carry), "+r" (a)  // 输出: carry, 输入输出: a
        : "r" (b)                // 输入: b
        : "cc"                   // 条件码寄存器被修改
    );
    return carry;
}

int check_sub_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        "sub %2, %1\n\t"      // a - b, 结果存入 %1
        "setc %0"             // 如果借位标志(CF)被设置,则 carry = 1
        : "=r" (carry), "+r" (a)  // 输出: carry, 输入输出: a
        : "r" (b)                // 输入: b
        : "cc"                   // 条件码寄存器被修改
    );
    return carry;
}

int check_mul_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int high_bits;
    unsigned char overflow;
    __asm__ volatile(
        "mul %2\n\t"          // eax * %2, 结果在 edx:eax
        "mov %%edx, %1\n\t"   // 高32位存入 high_bits
        "test %%edx, %%edx\n\t"  // 检查高32位是否为0
        "setnz %0"            // 如果不为0,则 overflow = 1
        : "=r" (overflow), "=r" (high_bits)  // 输出: overflow, high_bits
        : "r" (b), "a" (a)     // 输入: b, a(必须放在eax中)
        : "cc"                 // 条件码寄存器被修改
    );
    return overflow || (high_bits != 0);
}

int check_div_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char is_div_zero;
    __asm__ volatile(
        "test %2, %2\n\t"     // 测试除数 b 是否为 0
        "setz %0"             // 如果为零标志(ZF)被设置,则 is_div_zero = 1
        : "=r" (is_div_zero)   // 输出: is_div_zero
        : "r" (a), "r" (b)     // 输入: a, b
        : "cc"                 // 条件码寄存器被修改
    );
    return is_div_zero;
}

int main() {
    printf("(UINT_MAX + 1)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(UINT_MAX, 1)));   // 1
    printf("(1, 0)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(1, 0)));  
    printf("(0, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(0, 1)));          // 1
    printf("(2, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(2, 1)));
    printf("(UINT_MAX, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(UINT_MAX, 2)));   // 1
    printf("(1, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(1, 2)));
    printf("(10, 0)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(10, 0)));                         // 1
    printf("(2, 1)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(2, 1)));
    return 0;
}