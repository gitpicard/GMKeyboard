# GMKeyboard
Translates keys across different keyboard layouts for GameMaker Studio 1.4.

# Why?
We often forget that there are many different keyboard layouts with keys in totally different spots. Take for
example WASD. This works great on many keyboard layouts but it does not work at all on the French keyboard layout.
On a French keyboard, ZQSD takes the place of WASD.

It would be a pain to write code for every different keyboard layout would it not?

Well the solution is to use scancodes instead of keycodes for your keyboard input. unfortunately for us, GameMaker does
not support this and uses keycodes. This is where this extention comes in. It provides two functions, `from_user_layout` and
`to_user_layout` that allows you to determine what key is in what position. Say you want to use the arrow keys and Z for input.
Well on a German keyboard, the Y is in the Z position. Just call `chr(to_user_layout(ord('Z')))` and you will get `'Y'` as the
result.

# Limitations
Currently, this only works for the Latin alphabet characters.

# API
`from_user_layout(char)`
Takes the character value (obtained from `ord()`), looks at the user's keyboard, and determines what key is located
there on a US keyboard layout (which is the standard used for scancodes that are layout independent.)

`to_user_layout(char)`
Takes a character value (obtained from `ord()`), looks at a US keyboard layout (the standard used for scancodes) and translates
it to the key on the user's layout that is in the same physical position.
