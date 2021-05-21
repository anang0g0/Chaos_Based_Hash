require 'matrix'

gem 'ruby-numtheory'
require 'numtheory'

class IntegerModRingM
    def initialize(int, mod)
        @int = int % mod
        @mod = mod
    end
    def to_i
        @int
    end
    def abs
        @int
    end
    def to_s
        "#{@int}" # (mod #{@mod})"
    end
    def ==(other)
        @int == other.to_i
    end
    def +(other)
        new(@int + other.to_i)
    end
    def -(other)
        new(@int - other.to_i)
    end
    def *(other)
      case other
      when self.class, Numeric
        new(@int * other.to_i)
      else
        other * @int
      end
    end
    def **(other)
        new(@int * other.to_i)
    end
    def quo(other)
        return self if @int.zero?
        case other
        when Integer, self.class
            new(@int * other.to_i.inverse(@mod))
        when Rational
            new(@int * other.denominator * other.numerator.inverse(@mod) )
        else
            raise "not impl #{other.class}"
        end
    rescue ArgumentError => e
        raise "#{e} #{other} mod #{mod}"
    end
    alias_method :/, :quo
    def coerce(other)
        case other
        when Integer
            [new(other), self]
        when Rational
            [new(other.numerator).quo(other.denominator), self]
        else
            raise "not impl #{other}"
        end
    end

    private
    def new(int)
        self.class.new(int, @mod)
    end
end

def IntegerModRing(mod)
    class_name = "IntegerModRing#{mod}"
    return Kernel.const_get class_name rescue nil

    cls = Class.new(IntegerModRingM) do |cls|
        define_method :initialize do |int|
            super int, mod
        end
        define_method :new do |int|
            self.class.new(int)
        end
    end
    Kernel.const_set(class_name, cls)
    cls
end


#@CRV_name="eccp521"
@CRV_a= -3
@CRV_b=0x051953eb9618e1c9a1f929a21a0b68540eea2da725b99b315f3b8b489918ef109e156193951ec7e937b1652c0bd3bb1bf073573df883d2c34f1ef451fd46b503f00
@CRV_p=6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151
@CRV_n=6864797660130609714981900799081393217269435300143305409394463459185543183397655394245057746333217197532963996371363321113864768612440380340372808892707005449
@CRV_G_x=0x0c6858e06b70404e9cd9e3ecb662395b4429c648139053fb521f828af606b4d3dbaa14b5e77efe75928fe1dc127a2ffa8de3348b3c1856a429bf97e7e31c2e5bd66
@CRV_G_y=0x11839296a789a3bc0045c8a5fb42c7d1bd998f54449579b446817afbd17273e662c97ee72995ef42640c550b9013fad0761353c7086a272c24088be94769fd16650

# set G after definition of Point
#CRV = Struct.new(:a, :b, :p, :n, :G).new(@CRV_a, @CRV_b, @CRV_p, @CRV_n, [@CRV_G_x, @CRV_G_y])
CRV = Struct.new(:a, :b, :p, :n, :G).new(394716, 263144, 1063903, 1063891, [823140, 31315])

class EllipticCurvePoint
  attr_reader :x, :y

  def initialize(x, y, assert = false)
    @x, @y = [x, y]
    assert_on_curve! if assert
  end
  def on_curve?
    poly_x == poly_y
  end
  def assert_on_curve!
    raise "no point.\nx = #{x}\ny = #{y}" unless on_curve?
  end

  def ==(other)
    x == other.x && y == other.y
  end

  def to_s
    "EC(#{x}, #{y})"
  end

  # eadd
  def +(other)
    if other.is_a? Numeric
      if other.zero?
        return self
      else
        raise 'not impl'
      end
    end
    x1, y1 = [self.x, self.y]
    x2, y2 = [other.x, other.y]

    dx = (x2-x1) % CRV.p
    dy = (y2-y1) % CRV.p
    if dx == 0 || dy == 0
      raise "infinity devide #{self} #{other}"
    end

    #  X3=(Y3*Y3*Inv(X3*X3,mod,&Q)-X1-X2)%mod;
    q = dx.inverse(CRV.p)
    ll = dy * q % CRV.p   #lambda
    x3 = (ll * ll - x1 - x2) % CRV.p
    y3 = (ll * (x1-x3) - y1) % CRV.p
    if(x1==x3)
      raise "when this happens?"
      y3 = CRV.p - y1
    end

    self.class.new(x3, y3, true)
  end

  def edbl
    mod = CRV.p

    x1  = 3 * x**2 + CRV.a
    y1  = 2 * y
    x3 = y1.inverse(mod)
    ll = x3 * x1 % mod
    x2  = ll**2 % mod

    x3 = (x2-2 * x) % mod
    y2 = ll * (x-x3) % mod

    y3 = (y2 - y) % mod
    self.class.new(x3, y3, true)
  end

  #Compute Q=mG
  def *(m)
    #puts "in #{__method__}"

    # array of indexes of bit is 1
    # ex: 11 == 0b1011 -> [0,1,3]
    begin
      bit_index = m.to_i.to_s(2).chars.reverse.map.with_index{|b,i| i if b=='1'}.compact
    rescue ArgumentError
      raise m.to_s
    end
    mktable(bit_index.last)
    
    _Q = @table[bit_index[0]]
    for i in 1..bit_index.size-1
      le = @table[bit_index[i]]
      _Q += le
    end
    _Q
  end

  def coerce(other)
    [self, other]
  end

  private

  def poly_x
    (x**3 + CRV.a * x + CRV.b) % CRV.p
  end
  def poly_y
    y**2 % CRV.p
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
end

CRV.G = EllipticCurvePoint.new(CRV.G[0], CRV.G[1], true)


#-------------------

P=CRV.p

def randInt
    rand(P/10..P)
end

def randMatrix
    Matrix.build(2){IntegerModRing(P).new(randInt)}
end

class Alice
    def initialize
        @a = 1
        @b = 2
        @c = 1
        @d = 1
        @A = Matrix[[5,6],[7,8]].map{|x| CRV.G * x}
    end

    def public_keys
        @X = Matrix[[1,2],[3,4]].map{|x| IntegerModRing(P).new(x)}
        @Y = @X**@a * @A*@c * @X**-@a
        @Z = @X**@b * @A*(@c * @d) * @X**-@b
        [@X, @Y, @Z]
    end

    def c2_from_c1(_C1)
        @X**(@b - @a) * (_C1*@d) * @X**(@a - @b)
    end
end

class Bob
    def initialize(x, y, z)
        @X = x
        @Y = y
        @Z = z
    end
    def c1
        r = 1
        s = 1
        _C1 = @X**r * @Y*s * @X**-r
        @C2 = @X**r * @Z*s * @X**-r
        _C1
    end
    def c2
      @C2
    end
end


if caller.length == 0
    alice = Alice.new
    pub_keys = alice.public_keys
    bob = Bob.new(*pub_keys)

    _C1 = bob.c1
    puts bob.c2
    _C2 = alice.c2_from_c1(_C1)
    puts _C2

    if _C2 == bob.c2
        puts('success')
    else
        puts('fail')
    end
end
