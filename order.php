#!/usr/bin/php
<?php

// pick any 5 random notes from the set of [1, 2, 3, 4, 5, 6, 7]
$notes = array(1, 2, 3, 4, 5, 6, 7);
$count = 10;
$repeats = 2;
$ints = array();

$out = getNotes($notes, $count, $repeats);

for ($x = 1; $x < count($out); $x++) {
    $a = $out[$x - 1];
    $b = $out[$x];

    $ints[] = getInterval($a, $b);
}

echo implode(", ", $ints) . "\n";
echo implode(", ", $out);

function getInterval($a, $b) {
    if ($a > $b) {
        $tmp = $a;
        $a = $b;
        $b = $tmp;
    }

    // we're always going up
    $int = 1;

    for ($x = $a; $x < $b; $x++) {
        switch ($x) {
            case 1:
                // from 1 to 2
                $int += 2;

                break;
            case 2:
                // 2 to 3
                $int += 2;

                break;
            case 3:
                // 3 to 4
                $int += 1;
                
                break;
            case 4:
                // 4 to 5
                $int += 2;

                break;
            case 5:
                // 5 to 6
                $int += 2;

                break;
            case 6:
                // 6 to 7
                $int += 2;
                break;
        }
    }

    echo "A ($a) to B ($b) is $int frets.\n";
    return $int;
}

function getNotes($notes, $count, $repeats = false) {
    $ret = array();

    for ($x = 0; $x < $count; $x++) {
        $i = array_rand($notes);
        $cur = $notes[$i];
        $ret[] = $cur;

        if (!$repeats) {
            $notes = array_diff($notes, array($cur));
        } else if (is_int($repeats)) {
            $c = array_count_values($ret);

            if ($c[$cur] >= $repeats) {
                $notes = array_diff($notes, array($cur));
            }
        } 
    }

    return $ret;
}
