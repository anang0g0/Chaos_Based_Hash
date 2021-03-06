# Chaos_Based_Hash


原理：データバッファをコピーしてそれに置換と、3ビット右巡回シフトと、非線形変換（AESのS-box）したものをコピー前のバッファにXORしているつもり。

これはカオスというよりは、算術的ハッシュ関数です。
カオスはまだ勉強中なので定義も覚えてないし、その意味ではカオスのイメージに触発されてできたハッシュ関数もどきと言えそうです。
カオス理論を使った暗号というのは、大抵テント写像というものを使って説明がされてますが、私の場合はパイコネ変換しかしてません。


ハッシュ関数：https://ja.wikipedia.org/wiki/%E3%83%8F%E3%83%83%E3%82%B7%E3%83%A5%E9%96%A2%E6%95%B0

カオス理論：https://ja.m.wikipedia.org/wiki/%E3%82%AB%E3%82%AA%E3%82%B9%E7%90%86%E8%AB%96

パイコネ変換：https://ja.wikipedia.org/wiki/%E3%83%91%E3%82%A4%E3%81%93%E3%81%AD%E5%A4%89%E6%8F%9B

疑似カオス：https://core.ac.uk/download/pdf/147691564.pdf

# 20210521

置換乱数もそのままだと周期が見えてしまうので、ビットスワップを使うとさらにガウス化できるので、その方向で乱数は勧めている。

カオスの勉強。
最終的に数式を使っていない冗長な説明の本を読むか、数式は出てくるけど離散線形系の力学を中心にやる教科書にするか迷う。
一応自分の必要としているものがなんだかわかってきたので元気が出たら本格的に始めようと思う。

一昨日くらいまでは調子が一時的に良かったけど、昨日から今日にかけて中途覚醒してしまって、夕方になると猛烈に眠くなる。
椅子に座っているといつの間にか寝てしまうくらい眠い。
今日になってから味覚異常というか、豆腐の味が薬みたいに感じる。
うちに来る訪問看護でコロナが怖くないからという理由でマスクを半分ずらしてくる看護師がいて、もし私感染したらどうするつもりなのか。
コロナだったら最悪だ。

置換乱数を数式で説明できる。
演算は全て行列で表現できる。
xorshiftに力学が当てはまるか早く知りたい。

置換行列が全ての行を計算したあとでないと結果が更新されないところを、置換行列の一行毎に再帰的に参照元配列を更新するという仕組みになっていることが判明。
周期が長くなる場合があるけど、それで乱数を作ってみてもそんなに質が良くない。  
代わりにビットスワップの処理を入れるとガウス化されるようだ。(bit.c)  
これも数式で説明できないといけない。
分析するって楽しい。


# 20210519

MT最強！ｗ

私はMTをなめていた！ｗ

https://oupo.hatenadiary.com/entries/2014/10/16

でもMTは考えられた疑似乱数だった。
周期だけで言ってもxorshiftより上なのだ。
32×624ビットの状態空間で、20000ビット近い周期を持つのは妥当だと言える。
巨弾な素数を使えばそれ以上の周期を持つ乱数は作れるかもしれないけど、こんなにコンパクトで早いアルゴリズムは殆ど無いだろう。
これに勝てるとしたらカオスしかない。

ついでに言うけど、「線形力学系＝行列」と思いこんでいたけど全然違った。
むしろ「量子力学＝行列」だと思う。

置換乱数の最大周期を計算することが出来た。  
全く使い物にならない。  
２つの置換 a,b があった時、それぞれの巡回位数がｍ，ｎだったとすると、最大周期はｍｎになる。  
シフトが表現できないためで、本質的に置換軍を乱数に使うことの限界があるようだ。  

一方で置換も自己参照型書き換えをすると、つまりデータが格納されている配列をA,置換π=[2,3,4,5,6,7,0,1]として、  

A^=A[π]

と自分自身を書き換えながら置き換えてやると、その挙動が面白くなる。
このときの配列のパターンは３つに分けられる。

１．出力が０になる  
２．非周期的になる（バラバラ）  
３．周期的になる  

の３つの状態になり、かなりカオスっぽくなる。
たった８ビットの初期値しかなくても沢山のバリエーションがある。

# 20210518

線形力学系とxorshiftに関係があると予想。
今調べてます。

力学系の定義を知らないのでなんとも言えないが、多次元の力学系であって行列で説明がつく物理らしい。
非線形をやる前に、まず線形力学を理解しようと思う。
式に行列が出てくのは全部線形だろうか？
実世界の物理現象を表す計算は全部実数だろうから、有限体で計算する物理なんて言うのはありえないし無意味かもしれない。

今やろうとしているのはカオスではなく、（行列を使ってもカオスになるのかもしれないが）F_2^nの力学である。
解析学であるはずのカオスに代数が出てくるのは意外だ。
最早カオスではなく、線形代数で力学がどの様に記述されるのかに興味が移っている。
力学への応用色が強い線形代数の本が手元になるので、いい教科書が揃っているというべきか。

最初は因数分解のロー法とカオスにどんな関係があるかと不毛な計算を繰り返していたけど、もしかしたら整数を使っている時点で非線形でないのではと疑問を持つようになった。
そして線形力学を調べると行列で表される力学系のことだとわかって、xorshiftに応用できるのではないかと思い始めた。

BBCRSの論文には、置換行列の一般化として普通1しかない置換行列を、拡大体の元にするという一般化が行われた。
この行列がどのような力学系になるかを説明できるようになることを目標にする。
一般化しても逆行列は存在するし、何も問題がないように思えるが、それは明日以降のお楽しみだ。

# 20210517

10時間寝てやっと持ち直した。
午後から実験。
あまり面白そうな結果が出ない。
整数カオスは最早非線形物理ではないのかもしれない。

半年以内にデバネーの教科書をやる予定。

そんなわけで早々に実験を終わらせて、別の方向から乱数を見てみることにした。
というのも、整数カオスというのは実は非線形力学じゃないんじゃないかと思ったからだ。

そういえば私の置換乱数もそうだ。
果たして両者は本物のカオスを扱っているのだろうか？

そこで線形力学なんてものに関心を持ったので調べてみた。

線形力学：https://termoshtt.gitlab.io/chaos-da-book/dynamical_systems/linear.html

xorshiftの数理：https://blog.visvirial.com/articles/575

線形力学が行列で表すことのできる力学系だとするなら、
これは数学的にきれいな乱数を力学系として見做すという方法が正しいかどうかの試金石になると思う。
まずこの問題を解いてみよう。

# 20210516

ほとんど進展なし。
リ・ヨークの定理とシャルコフスキーの定理を知った。

https://wacchoz.hatenablog.com/entry/2019/01/05/230128

このページがとても参考になった。
ロー法もカオスかもしれないと思ったので調べる予定。

具合が悪い。


# 20210515

眠い。
疲れているんだと思うけど毎日眠い。

カオス理論なんてもう古いから、新しい結果なんてもう何も出てこないと言われた。
数理物理というか、数学理論としてカオスをやってみようと思ったのだが、確かにカオスの応用に関してネットで調べてみると全部スネークオイルっぽい。
正しい応用がされていないというか、素人でもそんな説明でいいの？って感じのことが沢山ある。
私がやりたいのは、有限桁カオスの周期についてなのだが、そういう研究はされてないのだろうか？
カオスと言っても、有限桁ならなんにでも周期はつきものだ。
それを理論でどういう性質を持つか、明らかにしたいというのが研究の動機なのだが、これってもうわかっていることなのだろうか？

カオス同期なんかで暗号化する技術があるということも本に載っている。
やはりネットの情報の信憑性には限界がある。

新しい結果なんて何も出てこないかもしれない。
でもそれがわかるだけでも幸せだと言えないだろうか？


# 20210514

息抜きになんか他のことをしてみたくなった。

# 20201207

コードをちょっときれいにしてみたつもりです。
後は見る人の好みですね。

AESのS-boxを使うというのはたしかに非線形変換ですが、このハッシュ関数に本質的に不必要なものだと思うので削除しました。
XOR演算の結果、バッファの値が全部消える可能性があるので、桁溢れによる一方向性を狙ってバッファに加算をする方法に代えました。
足しても0になる可能性がありますが、一方向性を作るために有効なのではないかという思いつきです。

きちんと評価しないといけませんが、コードの可読性を優先させたつもりです。（ポインタ解ってないです）

20200721

秘密鍵暗号の開発を別のリポジトリに移して独立させました。
引き続きこちらのリポジトリではハッシュ関数について開発を勧めていきます。

もしかしたらと思うところがあるんですが、KeccaKで採用されたスポンジ構造というのは、散逸系のカオスのことじゃないかと思ってみたり。
それと比べると自分のハッシュ関数は保存系のカオスだと。

やっぱりカオスは重要。
パイコネ変換っていうだけでカオス決定ですもんね。

20200720

https://ja.wikipedia.org/wiki/Advanced_Encryption_Standard

これによるとAESは既にある程度まで解読されているらしいので、私たちはさんざん大騒ぎした挙句、結局弱い暗号をつかまされたことになるかもしれないです。
その点のことを作者さんはどう思っているのでしょうか？

私の自作オレオレ暗号は、確かに自分のイメージ通りにはできてるけど、これってSPNのように見えて仕方がない。
あえて言えばFeistel構造がごっっちゃになってる感じ。

とある本に書いてあることが正しければ、秘密鍵暗号というものは置換と換字だけで作れるはずだ。
私のアプローチではFeistel構造のように左右を半分に分けて、その片方だけにF関数をかけるのではなく、置き換えの場所を固定せずに動的にすべてのブロックサイズに対して置換をかけるというものだ。
それが今の形に一番近いし、目指しているものだ。

これで安全性が高まるという根拠は全くない。
というか考えてない。
単に安全性が高まるかもしれないという希望的観測があるだけだ。
単に安全性検証が難しくなるだけかもしれない。
従来の混ぜるパターンを固定する方が解析しやすいし、それが安全性をクリアに検証できるならその方がいいと思う。
何の根拠もなく自分のやり方に固執するのはよくない。
だからオプションで自分のパターンを使うようにして、基本はSPNにしておくのがいいのかもしれない。


20200719

バグをとった。
サブキーを変化させてなかったのと、それをXORすると復元できないエラー。
ラウンドのかっこを閉じる位置が一行だけずれていた。
自分のイメージ通りには作れたけど、これがFeistel構造かと言われれば全然違う。

それはいいとして、一通りDESのFeistel構造を理解しているつもりが、自分のを作った辞典で全く生かされてないことに気がついた。
鍵に当たるバッファのサイズが256バイトって何？
全然ｆ関数じゃないしｗ。
もう一度設計からやり直しです。

20200718

自分で作った秘密鍵暗号が出来たっぽい。
まだ鍵スケジュールが適当だからなんとも言えないけど、速度的にはopensslのカメリアと同じくらい。
SPN構造を採用。
まだテキトーだから、絶対に安全じゃない。
それだけは期待しない。

20200717

Light Weight Cipherのつもり。
できることなら一般化Feistel構造にしたい。

LWC/lwc.c


20200716

AES:https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf

SHA-3:https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf

こんな風に、何でもかんでも書籍に頼らずFIPS嫁ってなったら日本語文化は死んでしまうというのに。
英語が公用語とはいってもまだ半分以上の人は高等教育受けてないし、勉強しない名ばかり大学生もいるし、横文字は苦手なんだから。
やはり英語より日本語のほうが解りやすいし、嫁といわれれば読むけど、読みたいかといわれるとちょっと考えさせてくださいって感じ。
日本語の説明だってFIPSのパクリばっかりだし。
書籍という媒体で、なんでSPNだとかスポンジ構造だと安全といえるのかしっかりわかりやすく解説する余地はまだあると思う。
SPNという意味はウィキに寄ると換字と置換の意味らしいけど、秘密鍵暗号の仕組みはその２つに限られるともブーフマンの本には書いてあったのを思い出す。
アフィン変換は破られやすいのに、非線形置換だといわれると安全なような気もする。
ヒル暗号も非線形だと安全になるんだろか？
アフィンだけど非線形だと安全になるのだろうか？
この際どこに欠陥があるかよく分らないSPNを使うより、一歩下がってFeistel revisitedしてもよさそうな気がする。
何十年も使われてきて、２つしか解読法が見つからなかったのだから、そこそこ安全な設計だったのではと思えるし。
何よりこれだけボリュームのあるホワイトペーパーを、iPadもないのに印刷して持ち歩くのって結構大変だし、画面で見るのも光っているから目が疲れるし。
もしかするとAESの最終候補の中にFeistel構造のものが残っているかもしれないからそれを調べてみたらTwoFishがそうだと分り早速ダウンロード。
とにかく紙の本が欲しい！

正式にはアナウンスしてないけど、秘密鍵暗号も作るつもりです。

TwoFish:https://www.schneier.com/academic/paperfiles/paper-twofish-paper.pdf

20200714（モード2,3に対する考察）

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
