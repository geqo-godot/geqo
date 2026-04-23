class_name TestPerson
extends Node

var _first_name :String
var _last_name :String

func _init(first_name :String, last_name :String):
	_first_name = first_name
	_last_name = last_name

func full_name() -> String:
	return _first_name + " " + _last_name
