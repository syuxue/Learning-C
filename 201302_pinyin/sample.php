<?php
require 'pinyin.class.php';

// �������ı���
pinyin::setcharset('gb2312');

// תΪ�ַ���Ϊƴ��
$str = '���,����! GB2312��û�С��G�������';
echo $str."\n";
//$str = iconv('UTF-8', 'GBK//IGNORE', $str);
$res = pinyin::convstr($str, pinyin::PY_SPACE);
//$res = iconv('GBK', 'UTF-8//IGNORE', $res);
echo $res."\n";

// ת��ƴ��Ϊ�ַ���
$pinyin = 'wang';
$charlist = pinyin::getgbchar($pinyin);
foreach ($charlist as &$char) {
	//$char = iconv('GBK', 'UTF-8', $char);
}
print_r($charlist);
