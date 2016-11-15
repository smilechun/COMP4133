<?php

$in = fopen($argv[1], 'r');

$map = [];

while (($line = fgets($in)) !== false) {
	$line = trim($line);
	if ($line == '') continue;
	$line = explode(' ', trim($line), 2)[1];
	foreach (explode(' ', $line) as $word) {
		$word = trim(strtolower($word), " \t\x0b.,[]();:?!-'\"");
		if ($word == '') continue;
		$map[$word] = isset($map[$word]) ? $map[$word] + 1 : 1;
	}
}
arsort($map);
var_export($map);