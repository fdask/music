<?php
class Config {
    public static $debug = false;
    public static $screenWidth = 80;
    public static $saveDir = "saves";
    
    public static function strToChr($str) {
        $ret = array();

        $bits = str_split($str);

        for ($x = 0; $x < count($bits); $x++) {
            $ret[] = ord($bits[$x]);
        }

        return $ret;
    }

    public static function waitForInput() {
        $input = '';

        $read = [STDIN];
        $write = null;
        $except = null;

        readline_callback_handler_install('', function() {});

        // Read characters from the command line one at a time until there aren't any more to read
        do {
            $input .= fgetc(STDIN);
        } while (stream_select($read, $write, $except, 0, 1));

        readline_callback_handler_remove();

        return $input;
    }

    public static function autoload($class) {
        $file = __DIR__ . "/" . str_replace('\\', '/', strtolower($class)) . ".inc";
        
        if (file_exists($file)) {
            require $file;
        } else {
            if (Config::$debug) {
                debug_print_backtrace();
            }
            
            die("Couldnt find file: $file\n");
        }
    }

    public static function randomWords($num = 3, $join = "") {
        $output = shell_exec("shuf -n$num /usr/share/dict/american-english");

        $words = preg_split("/\r\n|\n|\r/", $output);
        
        $ret = array();

        array_pop($words);
        
        foreach ($words as $word) {
            $bits = preg_split("/'/", $word);
            $word = strtolower($bits[0]);
            $ret[] = $word;
        }
        
        return implode($join, $ret);
    }

    public static function toTable($data) {
        $cols = count($data[0]);

        $charsPerCol = floor(Config::$screenWidth / $cols) - $cols;

        $rows = count($data);

        $ret = "";

        for ($row = 0; $row < $rows; $row++) {
            $r = $data[$row];

            for ($col = 0; $col < $cols; $col++) {
                if ($col > 0) {
                    $ret .= "|";
                }

                $c = $r[$col];

                if (strlen(Colors::stripColor($c)) > $charsPerCol) {
                    $ret .= substr($c, 0, $charsPerCol);
                } else if (strlen(Colors::stripColor($c)) == $charsPerCol) {
                    $ret .= $c;
                } else if (strlen(Colors::stripColor($c)) < $charsPerCol) {
                    $ret .= str_pad($c, $charsPerCol, " ");
                }
            }

            $ret .= "\n";
        }
        
        return $ret;
    }

    public static function drawTable($widths, $data, $trailingEdge = false) {
        $ret = array();
        
        // figure out the total numbers of lines
        $lines = 0;

        foreach ($data as $d) {
            if (count($d) > $lines) {
                $lines = count($d);
            }
        }

        for ($line = 0; $line < $lines; $line++) {
            $curStr = "";

            foreach ($data as $count => $d) {
                $lastColumn = ($count == count($data) - 1);

                if (isset($d[$line])) {
                    $str = Colors::stripColor($d[$line]);

                    if (Config::containsMb($str)) {
                        $diff = strlen($d[$line]) - strlen(utf8_decode($str));

                        if (strlen(utf8_decode($str)) <= $widths[$count]) {    
                            $print = str_pad($d[$line], $widths[$count] + $diff, " ");
                        } else {
                            $print = substr($d[$line], 0, $widths[$count] + $diff); 
                        }
                    } else {
                        if (strlen($str) <= $widths[$count]) {
                            $c = $widths[$count] - strlen(utf8_decode($str));

                            $print = $d[$line];

                            for ($cc = 0; $cc < $c; $cc++) {
                                $print .= " ";
                            }
                        } else {
                            $diff = strlen($d[$line]) - strlen(utf8_decode($str));
                            
                            $print = substr($d[$line], 0, $widths[$count] + $diff);
                        }
                    }
                } else {
                    $print = str_pad(" ", $widths[$count]);
                }

                // draw the column divider
                if (($lastColumn && $trailingEdge) || !$lastColumn) {
                    $print .= "|";
                }

                $curStr .= $print;
            }
            
            $ret[] = $curStr;
        }

        return $ret;
    }

    public static function containsMb($str) {
        return !mb_check_encoding($str, 'ASCII') && mb_check_encoding($str, 'UTF-8');
    }

    public static function addBorder($lines = array(), $color = 'bold_gray') {
        $ret = array();

        // get the widest line
        $maxLength = 0;

        foreach ($lines as $line) {
            $length = strlen(Colors::stripColor($line));

            if ($length > $maxLength) {
                $maxLength = $length;
            }
        }

        // print the top line
        $tmp = Ascii::upperLeft();

        for ($y = 0; $y < $maxLength; $y++) {
            $tmp .= Ascii::horizontal();
        }

        $tmp .= Ascii::upperRight();

        $ret[] = Colors::fg_color($color, $tmp);

        // add the end caps to each line
        foreach ($lines as $line) {
            if ($line == "") {
                continue;
            }

            $ret[] = Colors::fg_color($color, Ascii::vertical()) . str_pad($line, $maxLength, " ", STR_PAD_RIGHT) . Colors::fg_color($color, Ascii::vertical());
        }

        // print the bottom line
        $tmp = Ascii::lowerLeft();

        for ($y = 0; $y < $maxLength; $y++) {
            $tmp .= Ascii::horizontal();
        }

        $tmp .= Ascii::lowerRight();

        $ret[] = Colors::fg_color($color, $tmp);

        return $ret;
    }

    public static function splitText($len, $txt) {
        $ret = array();

        // split 
        $words = preg_split("@\s+@", $txt);

        $chars = "";
        $space = false;

        foreach ($words as $word) {
            // if there is room on the line (len - strlen($chars)
            if (strlen($chars) == 0) {
                $add = strlen($word);
                $space = false;
            } else {
                $add = strlen($word) + 1;
                $space = true;
            }

            if (($len - strlen($chars)) >= $add) {
                // add the char to the current line
                $chars = $chars . ($space ? " " : "") . $word;
            } else {
                $ret[] = $chars;

                // add the char to the first word of the next line
                $chars = $word;
            }
        }

        $ret[] = $chars;

        return $ret;
    }

    public static function drawLines($lines) {
        foreach ($lines as $line) {
            echo "$line\n";
        }
    }
}

spl_autoload_register("Config::autoload");

