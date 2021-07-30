
## C++ Implementation Details
* [x] possibly start using unique_ptr or shared_ptr instead of raw new/delete
* [ ] implement appropriate move and copy operators and constructors for each class
* [x] provide a method in `ini_file` to look up sections
* [x] provide a method in `ini_section` to look up entries
* [x] convert data structures `ini_file` and `ini_section` to maintain a map of keys for better access
* [x] change section and entry lookups to use map
* [x] *possibly provide a straight to entry method in `ini_file`*
* [x] provide a method for appending `ini_entries` to a section
* [x] provide a method for appending `ini_section` to a file
* [x] provide a serialization method for writing the `ini_file` out
* [x] move each class to its own file


## INI Implementation Details
* [ ] support hierarchical section names
* [ ] support C style backslash esacpes
* [x] support user defined comment deliminators
* [x] support user defined / locale defined line deliminators
* [ ] support multicharacter line terminators and character deliminators