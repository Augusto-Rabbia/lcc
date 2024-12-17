def is_prime(n):
    for i in range(2, n-1):
        if n % i == 0:
            return False
    return True

print(is_prime(257))

def PrimesUntil(n):
    Prime_list = ""
    for i in range(n+1):
        if is_prime(i):
            Prime_list += str(i)+" "
    return Prime_list

print(PrimesUntil(257))