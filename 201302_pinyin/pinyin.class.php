<?php

class pinyin
{
	const TABLE_GB2312	= 'table_gb2312.gb';
	const TABLE_GBK		= 'table_gbk.gb';

	const PY_TITLE		= 1;	// 拼音首字符大写
	const PY_UPPER		= 2;	// 拼音大写
	const PY_LOWER		= 4;	// 拼音小写
	const PY_INITIAL	= 8;	// 只保留首字母
	const PY_SPACE		= 16;	// 自动空格

	static private $_charset = 'GBK';
	static private $_table_c2p;
	static private $_table_p2c;

	public function setcharset($charset) {
		$charset = strtoupper($charset);
		if (in_array($charset, array('GBK', 'GB2312')))
			self::$_charset = $charset;
	}

	public function isgbk($high, $low) {
		$high = is_numeric($high) ? $high : ord($high);
		$low = is_numeric($low) ? $low : ord($low);

		if ($high < 0x81)
			return false; // fast return

		if ($high >= 0xA1 && $high <= 0xF7 && $low >= 0xA1 && $low <= 0xFE) {
			return $high <= 0xD7 ? 1 : 2;
		} else if ($high >= 0x81 && $high <= 0xA0 && $low >= 0x40 && $low <= 0xFE) {
			return 3;
		} else if ($high >= 0xAA && $high <= 0xFE && $low >= 0x40 && $low <= 0xA0) {
			return 4;
		} else if ($high >= 0xA8 && $high <= 0xA9 && $low >= 0x40 && $low <= 0xA0) {
			return 5;
		} else
			return false;
	}

	public function getgbchar($pinyin) {
		self::_table_init_p2c();

		$pinyin = strtolower($pinyin);
		if (isset(self::$_table_p2c[$pinyin]))
			return self::$_table_p2c[$pinyin];
		else
			return false;
	}

	public function convchar($gbchar) {
		self::_table_init_c2p();

		if (isset(self::$_table_c2p[$gbchar]))
			return self::$_table_c2p[$gbchar][0];
		else
			return false;
	}

	public function convchar_list($gbchar) {
		self::_table_init_c2p();

		if (isset(self::$_table_c2p[$gbchar]))
			return self::$_table_c2p[$gbchar];
		else
			return false;
	}

	public function convstr($string, $flag = self::PY_TITLE) {
		$result = '';
		$last_py = false;

		for ($i = 0, $len = strlen($string); $i < $len; $i++) {

			if (!isset($string[$i + 1]) || !self::isgbk($string[$i], $string[$i + 1])) {
				if ($flag & self::PY_SPACE && $last_py && !self::_isspace($string[$i]))
					$result .= ' ';
				$result .= $string[$i];
				$last_py = false;
			} elseif (($pinyin = self::convchar($string[$i].$string[$i + 1])) !== false) {
				// Auto space
				if ($flag & self::PY_SPACE && $i != 0 && !self::_isspace($string[$i - 1]))
					$result .= ' ';

				// Initial
				if ($flag & self::PY_INITIAL)
					$pinyin = substr($pinyin, 0, 1);

				// Upper/Lower
				if ($flag & self::PY_UPPER)
					$pinyin = strtoupper($pinyin);
				elseif ($flag & self::PY_LOWER)
					$pinyin = strtolower($pinyin);
				else
					$pinyin = ucfirst($pinyin);

				$result .= $pinyin;
				$last_py = true;
				$i++;				
			} else {
				if ($flag & self::PY_SPACE && $last_py && !self::_isspace($string[$i]))
					$result .= ' ';
				$result .= $string[$i].$string[$i + 1];
				$last_py = false;
				$i++;
			}
		}

		return $result;
	}

	private function _isspace($char) {
		return in_array($char, array(' ', "\n", "\r", "\t"));
	}

	private function _table_init_c2p() {
		if (isset(self::$_table_c2p))
			return;

		$tmp = file(constant('self::TABLE_'.self::$_charset));
		foreach ($tmp as $line) {
			$line = explode(' ', rtrim($line));
			$gbchar = array_shift($line);
			self::$_table_c2p[$gbchar] = $line;
		}
	}

	private function _table_init_p2c() {
		if (isset(self::$_table_p2c))
			return;

		$tmp = file(constant('self::TABLE_'.self::$_charset));
		foreach ($tmp as $line) {
			$line = explode(' ', rtrim($line));
			$gbchar = array_shift($line);
			foreach ($line as $pinyin) {
				if (isset(self::$_table_p2c[$pinyin]))
					self::$_table_p2c[$pinyin][] = $gbchar;
				else
					self::$_table_p2c[$pinyin] = array($gbchar);
			}
		}
	}

}
