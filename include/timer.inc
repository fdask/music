<?php
class Timer {
    private $start;
    private $stop;

    public function __construct() {
        $this->reset();
    }

    public function stop() {
        $this->stop = microtime(true);
    }

    public function getTime() {
        if (is_null($this->stop)) {
            $this->stop();
        }

        $ret = round($this->stop - $this->start, 2);

        $this->reset();

        return $ret;
    }

    public function reset() {
        $this->start = microtime(true);
        $this->stop = null;
    }
}