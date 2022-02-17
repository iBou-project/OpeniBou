# M5StickC Plus
[M5StickC Plus](https://www.switch-science.com/catalog/6470/)
M5StickC が入手できなくなったため Plus を使うようにした。

# Arduino
とりあえず Arduino ので開発。

# UI Flow
UI Flow はたぶんファームウェアを書き換える必要がある。
ペンディング。

# サンプルプログラム
## [buruburu](./buruburu)
ぶるぶる。M5StickC Plus に [GROVE - バイブレータ](https://www.switch-science.com/catalog/828/) をつけてみた。資料からはどこに配線されているかわからなかった。
G33 だった。

## [blynk-test](./blynk-test)
噂の Blynk 。iphone にインストールして通信してみた。BLE は現在未対応。
2022 Feb 時点で Blynk はちょっと使いづらい。terminal は出来なかった。
Blynk の流儀に従わなくてはならないので、トラブル発生時に何をしたらいいのか
わからない。Androido で BLE 使った方が楽。

プログラマじゃない人にはいいのかも。

## [presentation](./presentation)
加速度センサーと BLE Keyboard のテスト。電池食いすぎ。
M5StickC Plus を振り始めて、ボタンAを押すと(微妙なタイミング)
接続先の iPad にキーボードの DOWN を送信する。
ボタンBを押すと UP 送信する。

## [sounder](./sounder)
サウンダーのテスト。tone つかうとサウンダーで音階を奏でることができる。
