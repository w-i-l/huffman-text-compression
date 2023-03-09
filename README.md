<h1>Huffman compression</h1>
<h2>An algorithm to compress data using Huffman encode</h2>
<!-- <img src='https://user-images.githubusercontent.com/65015373/222278005-6ba91817-ec2c-4732-b96d-17492acae392.png'> -->
<br>
<hr>
<h2>About it</h2>

<p>Current program is based of <a href='https://github.com/w-i-l/huffman-coding'>Huffman encode</a> which ensure a good data compression. It can be use for both encoding and decoding a file.</p>

<br>
<hr>
<h2>How to use it</h2>

<p>It has an CLI using an intuitive menu:</p>
<ul>
    <li><b>Read from file</b> - reads the data from a file</li>
    <li><b>Encode data</b> - generates the Huffman encoding of the data</li>
    <li><b>Write data</b> - writes the input file encoded</li>
    <li><b>Decode data</b> - writes the input file decoded</li>
    <li><b>Display data</b> - shows how the symbols were encoded</li>
    <li><b>Display entropy</b> - shows the Shannon entropy along with the current encode entropy</li>
    <li><b>Test the files</b> - compares two given files and shows the content differences</li>
</ul>

<p>To <b>encode</b> a file you would read from a file, encode the data and then write to another file.</p>
<p>Note that the output file <b>should be</b> a binary one!</p>


<p>To <b>decode</b> a file simply run decode data.</p>


<h3>How to run</h3>
<p>Compile, here I'm using gcc.</p>

<p>After compiling, lunch the executable, depending on the os:</p>

<p>Windows</p>
<code bash>gcc test.c util.c code.c encode.c menu.c main.c -o main</code>
<br>
<code bash>main.exe</code>

<br>
<br>
<p>Linux</p>
<code bash>gcc test.c util.c code.c encode.c menu.c main.c -o main -lm</code>
<br>
<code bash>./main</code>


<br>
<hr>
<h2>How it works</h2>

<p>It reads all the symbols from a file and store them in a <b>data struct</b> array. The it adds all the missing nodes from the Huffman tree, and start applying the algorithm to encode.<br>
After completing the tree the program writes the dictionary as key - value pair, with the key being the symbol itself and the value its apparation probability.<br>
From a given file, all the characters are then written encoded in a buffer (for this project I choose a 32 bits array - int) and when the buffer is full it is written to the output file.</p>

<p>When decoding a file, it first reads the dictionary and with it parses all the file and writes the decoded version to an output file.</p>


<br>
<hr>
<h2>Tech specs</h2>

<p>The entire program has its functions documtented as comments above the their definitions.</p>
<p>A probably confusing thing would be the adding of a <b>char*</b> that represents the encoding of a symbol to a <b>bit array</b> that I will explain below.</p>

<p>All the functionality is implemented via <b>add_to_buffer</b> function that takes 3 parameters: </p>
<ol>
    <li><b>int buffer</b> - where it stores the bits</li>
    <li><b>int buffer_size</b> - that represents how many bits we have written
    <li><b>char* to_encode</b> - the string that needs to be added to buffer</li>
</ol>

<p>The function checks how many bits we can write from the <b>to_encode</b> parameter and based on that it returns coresponding.</p>
<p>The add part is done by first shifting the current index to left by 1 so we "allocate" a new bit in which we can write. The second part, the writing, implies <b>bitwise or '|'</b> with the current value of the <b>to_encode</b> parameter.</p>

<p>Assume that for the letter <b><i>a</i></b> we have the encode "10011" store as <b>char*</b> and our <b>buffer</b> is 0 and so its index.</p>

<p>Buffer representation: (for simplicity assume that our it has only 8 bits)<br></p>
<p>+-----------+</p>
<p>| 0000 0000 |</p>
<p>+-----------+</p>

<p>We take the first char of out string (from left to rigth) which is '1' and we shift out number by 1 and the bitwe oring the value with 1.</p>

<p>+-----------+</p>
<p>| 0000 0001 |</p>
<p>+-----------+</p>

<p>Now we continue the same way and the result would be: </p>

<p>+-----------+</p>
<p>| 0001 0011 |</p>
<p>+-----------+</p>

<br>
<hr>
<h2>Performance</h2>
<p>I have tested the program with a big file  - 365 MB - and the compression rate would be ~60%. The time required to all reading the file, encoding and decoding it was, on my machine, ~12 minutes.</p>
