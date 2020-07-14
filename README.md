# Chaos_Based_Hash


原理：データバッファをコピーしてそれに置換と、3ビット右巡回シフトと、非線形変換（AESのS-box）したものをコピー前のバッファにXORしているつもり。

これはカオスというよりは、算術的ハッシュ関数です。
カオスはまだ勉強中なので定義も覚えてないし、その意味ではカオスのイメージに触発されてできたハッシュ関数もどきと言えそうです。

ハッシュ関数：https://ja.wikipedia.org/wiki/%E3%83%8F%E3%83%83%E3%82%B7%E3%83%A5%E9%96%A2%E6%95%B0

カオス理論：https://ja.m.wikipedia.org/wiki/%E3%82%AB%E3%82%AA%E3%82%B9%E7%90%86%E8%AB%96

20200714（モード1,2,3に対する考察）

モード1：
CTRモードがハッシュ関数に使われているかどうか検索しても出てこないのでこのまま行きます。
こんな感じです。詳しいことが解るまで暫くこのままにします。
しかしまあこんなMD5みたいにアドホックに作っただけで、SHA256よりも早いのが出来てしまうとは。
このバージョンはデバッグコードなのでソルトやパディング処理はしてません。
一文字の入力に対して、データがどのようにバッファ内に拡散していくかの様子が解るようにしてあります。
CTRみたいにカウンタとしてxorshiftの６４ビットバージョンを使っているので、このまま使ってもまず簡単には衝突は起きないはずです。

v[i]^=Sbox[ROTL8(f[x1[i]],3)+c.ar[i]];

そもそもCTRなんてカウンタをハッシュしているのと同じものがハッシュ関数だなんて馬鹿げていますけども。
でもまあ平文にカウンタを足しているわけだし、何もないときとは違う出力になるだろうと予測できます。（デバッグコードなのでカウンタは空です）

２５６バイトの同じ文字の並びでも規則性のある出力が出てくるので、この関数には意味がないことがわかります。
すると次の方法を考える必要が有ります。

CTRを使わない他の方法も考えました。

最初のアイデア（モード2）：f[i]^=Sbox[ROTL8(f[x1[i]],3)+f[i]];

最初は苦し紛れだったんですが、置換の周期と関係あるかも。
基本的なアイデアはSHAの真似です。
どこが？
SHAは特定のブロック範囲でかき混ぜる関数を変えてるんですね。
そこでもっと撹拌パターンを動的に複雑にしたらどうかという思いつきだったんですが、かえって評価しにくくなってしまいました。

こんなこと言うと数学的にキレイなSHAに失礼ですね。
書き換えるブロックを固定しないで、置換群の変化とともに書き換えるブロックが変わるようにしました。

＋も使ってます。パディング処理もしました。（デバッグモードの今は無効にしてあります）

置換群の周期は１７ビットくらいあります。

あえて言うなら周期的自己書き換え（場所によっては2回とか3回繰り返す）で安全性を評価するのがちょっと難しいかも知れないです。
コードは一行なのに、背後にある理論がはっきりしていない。
数学的解析は、きちんと考えれば分かるのかも知れないですけど、こういう見通しの悪い設計は間違いを見つけにくいのでいい設計とは言えません。
GOSTだって余りキレイな構造してなかったけど、プロには分かるようです。
KECCKAKのアルゴリズムも調べないといけないですね。
もちろんカオスを使ったやつとか何か基礎になる方法を調べてマスターしないといけません。
今のところ、これがどんな性能を持っているのかは謎です。
何も分っていないのでゴミと同じです。
ただきちんと計算手順を追っていけば、衝突することなる文書のペアが見つけられると思うので、攻撃法も調べないといけないですね。

汚い設計は嫌なので、キレイな方も用意しました。
それは次のようなコードです。（モード3）

if(i%3==0){

  v[i] ^= Sbox[ROTL8 (f[x1[i]], 7)+f[(i+17)%NN]];

}else if(i%11==0){

　v[i] ^= Sbox[ROTL8 (f[x1[i]], 5)+f[(i+13)%NN]];

}else{

　v[i] ^= Sbox[ROTL8 (f[x1[i]], 2)+f[i]];

}

キレイなのか汚いのかわかりませんが、少しは見やすくなったのではないでしょうか。（え、違う？）

これも何か安全性を保証するような基礎がないのでボツです。

こうなると最早カオスとは言えません。
あえて言うなら私のアイデアがカオス的だとｗ。
これらの方法を考察してどれが一番良さそうかを決めるのに、約１週間から１ヶ月はかかると思っています。

置換群のサイズを６４バイトに縮小してみました。
省メモリバージョン。


20200713

おおっと！こりゃ重大な設計ミスだｗ。
同じ文字列読ませると0になるｗ。

CTRモードみたいな感じで解決してみました。
ただこれも本質的な解決なっているかどうかはまだわかりません。
なってない可能背が大きいです。

なぜならカウンタを導入することは、実質的にそのカウンタをハッシュしているだけという事だからです。
CTRモードが悪いといっているわけではありません。
正しく実装できていれば私の解決策でもいいという事になるのでしょうが、多分なってない。

その点、カウンタなんかを使っていないSHAのほうがまともな設計になっていると言える。（ちゃんと調べてない）

カオスを使ったハッシュ関数には、きちんとした理論的な根拠がありますが、私のにはありません。

その辺をどうしなければならないのかはこれからの課題です。
理論上カウンタのサイズが64ビットなので、2^64バイトサイズの大きさ以下のデータならハッシュ値が重なることはないと思うのですが小さすぎるでしょうか・・・。

カウンタは使ってみましたが、そのまま足すと面白くないし何故か遅いので、xorshiftを使ってみました。手抜きですね。
まあ簡単な算数みたいな設計。
この方が解析しやすいし、こんな簡単な作りじゃ安全なものは永遠に出来ないかも知れないですが、気づかないよりマシですね。
もっと正当な設計の仕方を模倣したほうがいいでしょう。

この行に注目してください。

v[i] ^= Sbox[ROTL8 (f[x1[i]]+c.ar[i], 3)];

こうしたのは＋が一方向だからです。
オーバーフローを起こした部分は消えてしまいます！

とはいうものの、まだパディングのいい方法が浮かびません。
前のバッファとの関係で下手にカウンタなんかを代入すると、ハッシュ値が消えてしまうという設計ミスが有ります。
この原因と対策を考えるのは後日にします。

http://inaz2.hatenablog.com/entry/2016/03/07/194034

こういうハイテク使ってる人もいるので、何も隠すところがないハッシュもどきは推測されやすいと思うのですが、
問題なのは同じハッシュ値を持つ別の入力を探すという事なのでそれが簡単でなければいいのですが。
そういう人ならこの関数がだめな理由もすぐわかるでしょうねｗ。

ちゃんと勉強しないとダメですね。（反省）


20200711

処理が遅かったので高速化しました。
ディスクアクセスが原因で大きなファイルの処理に時間が書かていましたが、
読み込みバッファのサイズを大きくしたことでより大きなサイズのファイルに対しても
高速にハッシュ値を計算できるようになりました。（chash.c）

細かい部分を直しました。

今日の結論：sha256sumと比べると気のせいか1秒くらい早いです。
速度的に見ると勝っているように見えます。データ攪拌も十分だし、性能面でも思った以上に早かったです。
sage-9.0.tar.gzをハッシュした結果、sha256で3.6秒程度、私ので2.8秒くらいなので勝ってます。
まだバグがありそうなので油断はできませんが・・・。

今後の実験によっては、もしかしたら「秘密鍵暗号はカオスの一種ではないか」という予測が正しいのかどうかが分るかもしれません。

何かわかりそうで解らなかったことが整理できた。誤差のないはずの計算機の中で、何がカオスの原因になりうるか？
物理現象では測定器の誤差だけでなく、どれだけ正確なデータを使っても微分方程式を計算するときにどうしても誤差が出てしまう。
その誤差が累積して結果が予測不可能になる。
秘密鍵暗号では誤差はないが、ブラックボックスとして秘密鍵が存在するので、その知り得ない秘密鍵の値がデータに影響して予測不可能な結果になるのだということ。

検証：のちに。（実験と理論的説明が必要。このくらいしないと本当に修士号持っているのか疑わしいですよねｗ。）

検証法：まずこの関数のミニチュア（32ビット）を作って、それがだめな理由を漸化式を使って明らかにする。例えば変数 

unsigned int a=1;

とした時に、１６次元の置換群を使って入れ替えをし、2ビットのブロック単位を１ビット巡回シフトし、更に４要素の適当な配列を使って２ビット単位に変換したものをaにXORする。
巡回シフトが意味を持つためには単一ビットのみの操作では意味がないので、４ビットとか２ビットとかある程度まとまりをもったブロック単位で処理しなければならない。
その時、ブロック単位の数と置換群の次元を対応させる必要がある。
問題なのは、XORしているためある確率で突然すべてが０になってしまう可能性がある。
これを防ぐために+などの演算に処理を変える必要があるかも知れない。
こう考えるとセルオートマトンのライフゲームみたいに思えてきますねｗ。
乱数ではないのでハッシュ値の衝突が起きる確率が無視できる程度であれば合格になるはず。

後日やってみる攻撃：https://ja.wikipedia.org/wiki/%E8%AA%95%E7%94%9F%E6%97%A5%E6%94%BB%E6%92%83

こういう風に、数式で考えるといかにダメな設計かわかってきますね。

こういうものは人に尋ねる前に冷静になって、自分で納得いくまで考える必要を感じる。

参考文献：DES is not a group


20200710

ハッシュになっていないことがわかったので、修正しました。

それどころかカオス暗号は弱いという定評が出ていて、ウィキなんかにも悪い暗号として記載されていました。

では、普通のブロック暗号で計算していることは一体何なのでしょうか？
もし、ブロック暗号の計算がカオス理論で説明できないのであれば、それは一体何を計算していることになるのだろうか？

失敗の原因ですが、バタフライ効果が発揮できていませんでした。
実装ミス。
理想の方法が見つかってからも、同じ出力を持つ別の入力データがないかどうか攻撃します。

20200709

置換群を使ったパイコネ返還からカオスを作り、バタフライ効果でハッシュ関数を作ってみました。
安全性の検証はまだこれからです。
失敗作の予感がしますｗ。
