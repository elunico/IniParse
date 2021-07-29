# INI Parser

This implements a simple (and specific) `.ini` file parser in C++. 

`.ini` files tend to have some variation and so the exact specification of `.ini` file will be described here.

#### Line terminator &amp; Character Deliminator
The parser allows the user to define the character used for line terminating called the "line terminator". This character is referred to 
by this name throughout the document. It is, by default, `\n`.

A similar idea is character deliminator. These are a set of characters which 
define the beginning of a comment (and are themselves considered a part of that
comment). By default they are `#` and `;` and they are referred to by name later in the document as well

### Entries
Entries are the most basic part of the `.ini.` file. They consist
of a **key** and a **value** separated by an equal sign. 
There are limitations on what characters can be present in a key or value. 

Rules for keys:
  1.  keys cannot contain `=`, or the characters defined to be comment deliminator or line terminators. (see above section)
  2.  keys cannot **start with** `[`
  3.  all whitespace in a key (that is not a line terminator since a line terminator is 
      invalid in a key) is considered a part of the key
  4.  keys end at the equal sign delimintating the value. The
      equal sign is not a part of either the key or value, as 
      it is a deliminator
      
Rules for values:
  1.  Values cannot contain the characters defined to be comment deliminator or line terminators. (see above section)
  2.  all whitespace in a value (that is not a line terminator since a line terminator is
      invalid in a value) is considered a part of the value
  3.  values end at the line terminating character or comment deliminating character indicating 
      the start of a line comment 
      
#### Value Typing

`.ini` files are not typed and all keys and values are considered 
strings. That being said, there is the `T tom::ini_entry::adapt_value<T, AdapterFunc>(AdapterFunc adapter = adapt_to<T>{ }) const;`
method on `tom::ini_entry`. This method is templated on the type to convert the value to, T, and the function object that performs the conversion, AdapterFunc.
AdapterFunc can be any type including a lambda so long as it implements `T operator()(std::string s);`.

The `tom::ini_entry` class defines an inner struct called `tom::ini_entry::adapt_to<T>` which provides some 
basic default conversions using `std::stringstream`s. This is the default argument provided to the method and does not 
need to be invoked explicitly. Implementing `std::stringstream& operator<<(std::stringstream& s, T const& t)` on 
your types can allow them to be converted from strings directly using the struct and default argument

### Sections
Entries may, need not be, part of a section. If entries are 
parsed out of a file **outside** of a section, they are 
added to the `<Default Section>` section. 

Sections are indicated by `[` and`]` and a `\n` terminator. 
They must precede the entries, they contain. 

There is no way to end a section syntactically. All entries
under a section start are considered a part of that section. 
The section ends **only** when the next one begins. As a 
consequence, only entries that appear in the file at the 
beginning before the first section can be outside of a section.

### Comments 
All comments are line comments. They go from the comment 
start indicator to the end of the line (specifically the line terminator) 
character.

Line comments are indicated using either `#` or `;` by default, but 
this can be user-customized in the `tom::ini_parser` constructor. They
may appear anywhere in a line including after sections or 
entries or right at the beginning 

Comments may contain any character that is not the line terminator (see above).

Comments are stripped out when parsing so the resulting 
data structure will not contain comments. 

### Blank lines
Blank lines in the `.ini` file are ignored 

