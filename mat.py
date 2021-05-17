
# https://blog.visvirial.com/articles/575

N = 32
 
# Returns a * b.
def arr_mul(a, b):
    n = [0 for i in range(N)]
    for i in range(N):
        for k in range(N):
            n[i] = (n[i] ^ (((a[i] >>  k) & 1) * b[k]))
    return n
 
# Compute m^a.
def arr_pow(m, a):
    if a == 0:
        return [1 << i for i in range(N)]
    r = arr_pow(m, a//2)
    r = arr_mul(r, r)
    return arr_mul(r, m) if a%2==1 else r
 
# Returns (I + L^a).
def xorshift_l(a):
    return [(1 << i) | ((1<<(i+a)) if 0 <= i+a < N else 0) for i in range(N)]
# Returns (I + R^a).
def xorshift_r(a):
    return xorshift_l(-a)
 
# Get (n = a0 * a1 * ...) as an array [a0, a1, ...].
def factors(n):
    a = []
    i = 2
    while i*i <= n:
        if n%i == 0:
            a.append(i)
            n //= i
        else:
            i += 1
    a.append(n)
    return a
 
def xorshift32_test(a, b, c):
    t = arr_mul(arr_mul(xorshift_l(a), xorshift_r(b)), xorshift_l(c))
    I = [1 << i for i in range(N)]
    if arr_pow(t, 2**N-1) != I:
        return False
    for a in factors(2**N-1):
        if arr_pow(t, (2**N-1)//a) == I:
            return False
    return True
 
print("OK" if xorshift32_test(15, 17, 13) else "NG")
 