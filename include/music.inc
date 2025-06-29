<?php
class Music {
    public static function getInterval($a, $b) {
        if ($a == $b) {
            return 0;
        }
    
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
                case 7:
                    // 7 to 1?
                    $int += 1;
                    break;
            }
        }
    
        return $int;
    } 

    public static function IntervalOfNotesOn($string, $fret, $shape) {
        switch ($shape) {
            case 'C':
                switch ($string) {
                    case 1:
                        // low E
                        return array(3, 5);

                        break;
                    case 2:
                        // its either a root, or a fifth
                        return array(1, 6);

                        break;
                    case 3:
                        return array(2, 3);

                        break;
                    case 4:
                        return array(5, 6);

                        break;
                    case 5:
                        return array(1, 2);

                        break;
                    case 6:
                        // high e
                        return array(3, 5);

                        break;
                }
                
                break;
            case 'A':
                break;
            case 'G':
                break;
            case 'E':
                break;
            case 'D':
                break;
            default:

        }
    }

    public static function getNotes($notes, $count, $repeats = false) {
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
}