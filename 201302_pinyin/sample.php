<?php
require 'pinyin.class.php';

// 设置中文编码
pinyin::setcharset('gb2312');

// 转为字符串为拼音
$str = '你好,世界! GB2312中没有“翯”这个字';
echo $str."\n";
$str = iconv('UTF-8', 'GBK//IGNORE', $str);
$res = pinyin::convstr($str, pinyin::PY_SPACE);
$res = iconv('GBK', 'UTF-8//IGNORE', $res);
echo $res."\n";

// 转换拼音为字符串
$pinyin = 'wang';
$charlist = pinyin::getgbchar($pinyin);
foreach ($charlist as &$char) {
	$char = iconv('GBK', 'UTF-8', $char);
}
print_r($charlist);
