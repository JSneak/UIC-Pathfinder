const calculate = require("my_addon");



console.time('c++');
calculate.calc();
console.timeEnd('c++');

// console.log(calculate.calc());