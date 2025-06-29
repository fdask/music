#!/usr/bin/php
<?php
include 'include/config.inc';

$r = unserialize(file_get_contents("1739714727"));

$s = new Stats();
$ss = clone $s;

echo $s->getStats();

$s->addRecord($r);
$s->recalc();

$new = $s->getTotalSeconds();
$old = $ss->getTotalSeconds();

echo "Old is $old and new is $new\n";
echo $s->getStats($ss);
