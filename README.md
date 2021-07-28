# INI Parser

This implements a simple (and specific) `.ini` file parser in C++. 

`.ini` files tend to have some variation and so the exact specification of `.ini` file will be described here.

### Entries
Entries are the most basic part of the `.ini.` file. They consist
of a **key_** and a **value_** separated by an equal sign. 
There are limitations on what characters can be present in a key_ or value_. 

Rules for keys:
  1.  keys cannot contain `=`, `\n`, `#` or `;`
  2.  keys cannot **start with** `[`
  3.  all whitespace in a key_ (that is not `\n` since `\n` is 
      invalid in a key_) is considered a part of the key_
  4.  keys end at the equal sign delimintating the value_. The
      equal sign is not a part of either the key_ or value_, as 
      it is a deliminator
      
Rules for values:
  1.  Values cannot contain `\n`, `#` or `;`
  2.  all whitespace in a value_ (that is not `\n` since `\n` is
      invalid in a value_) is considered a part of the value_
  3.  values end at the `\n` character or `#` or `;` indicating 
      the start of a line comment 
      
`.ini` files are not typed and all keys and values are considered 
strings. 

### Sections
Entries may, need not be, part of a section. If entries are 
parsed out of a file **outside** of a section, they are 
added to the `<Default Section>` section. 

Sections are indicated by `[` and`]` and a `\n` terminator. 
They must precede the entries, they contain. 

There is no way to end a section syntactically. All entries
under a section start are considered a part of that section. 
The section ends **only** when the next one begins. As a 
consequence, only enties that appear in the file at the 
beginning before the first section can be outside of a section.

### Comments 
All comments are line comments. They go from the comment 
start indicator to the end of the line (specifically `\n`) 
character.

Line comments can be indicated using either `#` or `;`. They
may appear anywhere in a line including after sections or 
entries or right at the beginning 

Comments may contain any character that is not `\n`.

Comments are stripped out when parsing so the resulting 
data structure will not contain comments. 

### Blank lines
Blank lines in the `.ini` file are ignored 

