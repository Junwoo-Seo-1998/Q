Author : Yoonki Kim
Explanation : how to use *.cfg file and Load it
If wants more key contact to author

/////////How to write *.cfg/////////
Line 1 : Example KeyPress/A				//First binding
Line 2 : Example2 KeyDown/b MButtonPress/Left		//Second binding
			-In Example.cfg file

At second binding, it's name is "Example2" - so corresponding callback name have to be identical and it has two key binds (Keyboard B button is Down, Left MouseButton Press)
If binding has multiple bindings, it have to be written in one line

One file can have multiple bindings and one bindings can have multiple binds

Structure
=> "BindingName" "InputType" "KeyValue"

/////////Possible Structures/////////
BindingName
=> Don't have to worry about it, just have to make callback name to be identical

InputType  (!!!CASE SENSITIVE!!!)
=> 	"KeyPress"
	"KeyRelease"
	"KeyDown"
	"MButtonPress"
	"MButtonRelease"
	"MButtonDown"

"KeyValue" (!!!Some of Thems are CASE SENSITIVE!!!)

=>Keys 		"Space" or "SpaceBar"
		"L_Ctrl"
		"ESC" or "Esc"
		Function Keys... (F1 - F12)
		(Or just single character like A! these are not case sensitive)
	
=>MButtons	"Left"
		"Right"
		"Wheel"

/////////How to load/////////
=>In code
Engine::GetEngineComponent<InputManager>().LoadBindings("Example.cfg");

