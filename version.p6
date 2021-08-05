#! /usr/bin/perl6
(open 'src/version.h', :w).say("$/<pre>" ~ "{$/<patch> + 1}")
    if slurp('src/version.h') ~~ / $<pre> = [ <-[\d]>+? ]
        $<patch> = [ \d+ ] <ws> /
