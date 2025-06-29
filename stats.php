#!/usr/bin/php
<?php
include 'include/config.inc';

$s = new Stats();

echo $s->getStats();