def is_power_of_2(n):
    for i in range(n):
        if 2**i == n:
            return True
    return False

print(is_power_of_2(16))
print(is_power_of_2(64))
print(is_power_of_2(62))

def sum_powers_of_two_between(N,M):
    Sum = 0
    for i in range(N,M+1):
        if is_power_of_2(i):
            Sum += i
    return(Sum)

print(sum_powers_of_two_between(2,8))