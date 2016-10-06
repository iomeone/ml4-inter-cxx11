ML<sup>4</sup> Interpreter CEK machine edition version 1.
======

This is a practise implementing [tiny ML interpreter][ML4] on the [CEK machine][CEK].
The interpreter is written in C++11 language.

 [ML4]: http://www.fos.kuis.kyoto-u.ac.jp/~igarashi/class/isle4-06w/text/miniml003.html
 [CEK]: https://www.cs.indiana.edu/cgi-bin/techreports/TRNNN.cgi?trnum=TR197

ML<sup>4</sup> treats lambda calculus and
limited fixnum integer arithmetic (addition, multiple, less than comparison only)
under the small set grammar from Ocaml's one.

    $ make ml4
    $ ./ml4
    let rec fact = fun n -> if n < 1 then 1 else n * fact (n + -1) in fact 9;;
    val it = 362880
    #quit;;

License
------

The BSD 3-Clause

Copyright (c) 2016, MIZUTANI Tociyuki
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
