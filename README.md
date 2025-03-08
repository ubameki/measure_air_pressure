<div id="top"></div>

## 開発環境

<img src="https://img.shields.io/badge/-Arduino-00979D.svg?logo=arduino&style=plastic">

## 目次

1. [研究背景]
2. [製作目的]
3. [環境]
4. [使用方法]

## 1.研究背景

このプログラムは新しい片頭痛アプリ、機器の開発のために製作した。
現在多くの片頭痛を予測するwebサイトやアプリでは市町村単位で気圧を測定し、片頭痛予測を行っている。この方法だと現在地の気圧と齟齬ができてしまう。また、飛行機に乗るなど高度が変わると対応できない。このような事象に対応するために新たに現在地の気圧を測定して片頭痛予測を行うアプリ、機器を開発する。

## 2.製作目的
このプログラムは新しい片頭痛アプリ、機器の開発の第一歩として製作した。
実際に既存のwebサイトやアプリと現在地でどれほど気圧に差があるのかを調べるためのものだ。

## 3.環境

M5stack coreS3 SE での使用を想定している。センサはM5Stack用BME688搭載環境センサユニット（ENV Pro） 。SDカードは16GB以下のみ使用可能。

<!-- M5stack coreS3 SE の公式サイト-->
<p>
 <a href="https://docs.m5stack.com/ja/core/M5CoreS3%20SE">M5stack coreS3 SE 公式サイト</a>
</p> 

<p>
 <a href="https://www.switch-science.com/products/9274?srsltid=AfmBOopKG6JUK_u1f9jDzl1Fz4MX5i4BYSxoiXZZQckyQUE5moOJ9e7O">ENV Pro 公式サイト</a>
</p>

## 4.使用方法

YOUR WIFI SSID NAMEとYOUR WIFI PASSWORDを自分の環境のものに変更して使用する。
