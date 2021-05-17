def gcd(a, b):
    if a<b:
        return gcd(b,a)
    r = b
    while a%b!=0:
        r = a%b
        a,b = b, r
    return r

def f(x):
    return x*x+1

N = 761 # 652177 #2**(2**6)+1 
print(N)
for j in range(N):
    count=0
    x=j
    y=j
    z=j
    for i in range(N):  #range(10**7):
        #x = f(i) % N
        #y = f(f(i)) % N
        z = f(f(f(z))) % N
        x = f(x) % N
        y = f(f(y)) % N
        if x!=y:
            g = gcd(x-y, N)
        if x==j:
            print("same",x,i,g)

        #if z < j and j < x and x < y:
            #if i==f(i):
        #    print(j,x,y,z,i,g)
            #count=count+1
        #if 1<g<N:
        #    print(g,j)
        
            break
            if x==j:
                print ("precious")
    #if count > 0:
    #    print(count,j)

else:
    print("Fail")