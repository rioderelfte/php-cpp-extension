<?php

$funs = array(
    'helloWorld',
    'helloWorld2',
    'helloWorld3',
    'helloWorld4',
);

$refStr = "";

foreach ($funs as $fun) {
    $ref = new ReflectionFunction($fun);
    $refStr .= $ref . "\n";
}

echo $refStr;

function helloWorld5($name, $flag) {
    if (!$flag)
        return null;

    return "Hello World!!!elf and hello to " . $name . ", lol";
}

var_dump(helloWorld("Froscon", true));
var_dump(helloWorld2("Froscon", true));
var_dump(helloWorld3("Froscon", true));
var_dump(helloWorld4("Froscon", false));
var_dump(helloWorld4("Froscon", true));
var_dump(helloWorld5("Froscon", false));
var_dump(helloWorld5("Froscon", true));
var_dump(helloWorld6(false));
var_dump(helloWorld6(true));
