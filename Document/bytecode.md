# ByteCode specification

## ByteCode

### <code>move</code>
Copy the value in source register into destination one.
<pre>
move $1, $2
</pre>

### <code>print</code>
Print the value in the source register.
<pre>
print $1
</pre>

### <code>store</code>
Store the immediate value into the register.
The following code store the string value into register 1.
<pre>
store $1 "Hello, world !!"
store $2 0x1234
</pre>
