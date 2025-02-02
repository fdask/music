<?php
class Keypress {
    public static function backspace() {
        return chr(127);
    }
    
    public static function spaceBar() {
        return chr(32);
    }

    public static function delete() {
        return chr(27) . chr(91) . chr(51) . chr(126);
    }

    public static function escape() {
        return chr(27);
    }

    public static function enter() {
        return chr(10);
    }  

    public static function end() {
        return chr(27) . chr(91) . chr(70);
    }

    public static function home() {
        return chr(27) . chr(91) . chr(72);
    }

    public static function pageUp() {
        return chr(27) . chr(91) . chr(53) . chr(126);
    }

    public static function pageDown() {
        return chr(27) . chr(91) . chr(54) . chr(126);
    }

    public static function shiftUpArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(65);
    }

    public static function upArrow() {
        return chr(27) . chr(91) . chr(65);
    }

    public static function doubleUpArrow() {
        return chr(27) . chr(91) . chr(65) . chr(27) . chr(91) . chr(65);
    }
    
    public static function downArrow() {
        return chr(27) . chr(91) . chr(66);
    }

    public static function shiftDownArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(66);
    }

    public static function doubleDownArrow() {
        return chr(27) . chr(91) . chr(66) . chr(27) . chr(91) . chr(66);
    }

    public static function leftArrow() {
        return chr(27) . chr(91) . chr(68);
    }

    public static function doubleLeftArrow() {
        return chr(27) . chr(91) . chr(68) . chr(27) . chr(91) . chr(68);
    }

    public static function shiftLeftArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(68);
    }

    public static function rightArrow() {
        return chr(27) . chr(91) . chr(67);
    }

    public static function doubleRightArrow() {
        return chr(27) . chr(91) . chr(67) . chr(27) . chr(91) . chr(67);
    }

    public static function shiftRightArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(67);
    }
}