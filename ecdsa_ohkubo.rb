# Copyright(C) 2003 by tcshacina (20031230)
# Elliptic Curve Domain of Parametor from www.secg.org
# This is a kind of a ECDSA's imprementation
# This program will includes ECNR imprementation
# FreeECDSA ver0.1 CRV.n is prime but not so good prime.
# 

#require_relative 'init_curve'
#init_curve(521)

#"{0E8920FD-B914-4","P-521","p=2^521-1",
@CRV_name="eccp521"
@CRV_p=6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151
@CRV_seedE=0xd09e8800291cb85396cc6717393284aaa0da64ba
@CRV_r=0x0b48bfa5f420a34949539d2bdfc264eeeeb077688e44fbf0ad8f6d0edb37bd6b533281000518e19f1b9ffbe0fe9ed8a3c2200b8f875e523868c70c1e5bf55bad637
@CRV_a= -3
@CRV_b=0x051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00
@CRV_G_x=0x0c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66
@CRV_G_y=0x11839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650
@CRV_n=6864797660130609714981900799081393217269435300143305409394463459185543183397655394245057746333217197532963996371363321113864768612440380340372808892707005449

require 'digest/sha1'

# set G after definition of Point
CRV = Struct.new(:a, :b, :p, :n, :G).new(@CRV_a, @CRV_b, @CRV_p, @CRV_n, nil)

class Point
  attr_reader :x, :y

  def initialize(x, y, assert = false)
    @x, @y = [x, y]
    assert_on_curve! if assert
  end
  def poly_x
    (x**3 + CRV.a * x + CRV.b) % CRV.p
  end
  def poly_y
    y**2 % CRV.p
  end
  def on_curve?
    poly_x == poly_y
  end
  def assert_on_curve!
    raise "no point.\nx = #{x}\ny = #{y}" unless on_curve?
  end


  # eadd
  def +(other)
    x1, y1 = [self.x, self.y]
    x2, y2 = [other.x, other.y]

    dx = (x2-x1) % CRV.p
    dy = (y2-y1) % CRV.p
    if dx == 0 || dy == 0
      if self.on_curve?
        print "X1 on Curve\n"
      end
      if y2.on_curve?
        print "X2 on Curve\n"
      end
      raise "infinity devide #{self} #{other}"
    end

    #  X3=(Y3*Y3*Inv(X3*X3,mod,&Q)-X1-X2)%mod;
    q = inv(dx, CRV.p)
    ll = dy * q % CRV.p   #lambda
    x3 = (ll * ll - x1 - x2) % CRV.p
    y3 = (ll * (x1-x3) - y1) % CRV.p
    if(x1==x3)
      raise "when this happens?"
      y3 = CRV.p - y1
    end

    Point.new(x3, y3, true)
  end

  def edbl
    mod = CRV.p

    x1  = 3 * x**2 + CRV.a
    y1  = 2 * y
    x3 = inv(y1, mod)
    ll = x3 * x1 % mod
    x2  = ll**2 % mod

    x3 = (x2-2 * x) % mod
    y3 = ll * (x-x3) % mod

    three = Point.new(x3, y3)
    if(three.x == 0 && three.on_curve?)
      raise "hakken! #{three}"
    end

    y4 = (y3 - y) % mod
    Point.new(x3, y4, true)
  end

  # make a calcuration table from base point Q
  # in most case q is CRV.G so we should cache the table
  def mktable(max)
    #puts "in #{__method__}"
    
    @table = Array.new(max + 1)
    @table[0] = self

    for i in 1..max
      q = @table[i - 1].edbl
      raise "Y ga 0 #{i}" if q.y.zero?
      @table[i] = q
    end
  end

  #Compute Q=mG
  def ellip(m)
    #puts "in #{__method__}"

    # array of indexes of bit is 1
    # ex: 11 == 0b1011 -> [0,1,3]
    bit_index = m.to_s(2).chars.reverse.map.with_index{|b,i| i if b=='1'}.compact
    mktable(bit_index.last)
    
    _Q = @table[bit_index[0]]
    for i in 1..bit_index.size-1
      le = @table[bit_index[i]]
      if _Q == le
        raise "dbl: #{_Q.edbl}"
      end
      _Q += le
    end
    _Q
  end
end

CRV.G = Point.new(@CRV_G_x, @CRV_G_y, true)


# invert of integer in mod n
def inv(a, n)
  d = n
  x = 0
  s = 1
  while (a != 0)
    q = d / a

    r = d % a
    d = a
    a = r

    t = x - q * s
    x = s
    s = t
  end
  (x + n) % (n / d)
end

=begin
ECDSA signature eneration and verification
To sign a meddahe m, an entry A with domain parameters D=(q,FR,a,b,G,n,h)
and associated key pair (d,Q) does the following

1. Select a random or pseudorandom integer k, 1<=k<=n-1
2. Compute kG=(x1,y1) and r=x1 mod n. If r=0 then goto step1.
3. Compute k^{-1} mod n.
4. Compute e=sha1(m)
5. Compute s=k^{-1}(e+dr)mod n. If s=0 then goto step1.
6. A's signature for the message m is (r,s).
=end
def ecdsa(e, d)
  puts "in #{__method__}"

  r = s = nil
  loop do
    k = rand(1..CRV.n)
    puts "k = #{k}"
    r = CRV.G.ellip(k).x % CRV.n
    next if r.zero?

    k_inv = inv(k, CRV.n)
    s = (k_inv * (e + d*r)) % CRV.n
    next if s.zero?
    break
  end
  [r, s]
end

=begin
ECDSA signature velification
1. Verify that r and s are integers in the interval[1,n-1]
2. Compute e=sha1(m)
3. Compute w=s^{-1} mod n.
4. Compute u1=ew mod n and u2=rw mod n.
5. Compute X=u1G+u2Q. If X=O, then reject the signature. Otherwise, compute
v=x1 mod n where X=(x1,y1)
6. Accept the signature if and only if v=r.
=end
def verify(r, s, e, _Q)
  puts "in #{__method__}"

  w = inv(s, CRV.n)
  u1 = e*w % CRV.n
  u2 = r*w % CRV.n

  _X = CRV.G.ellip(u1) + _Q.ellip(u2)

  if(_X == Point.new(0,0))
    print "equal point\n"
    exit()
  end

  v = _X.x % CRV.n
  v == r
end

# d: private key
# Q: public key
# [r,s]: sign for e
def main()
  puts ARGF.filename
  hash = Digest::SHA1.hexdigest(gets)
  puts "hash = #{hash}"
  e = hash.hex

  d = rand(1..CRV.n)
  r, s = ecdsa(e, d)
  puts "ECDSA Signature r = ", r
  puts "ECDSA Signature s = ", s
  
  _Q = CRV.G.ellip(d)
  if verify(r, s, e, _Q)
    print "That's true!\n"
  else
    print "baka\n"
  end
end

main
