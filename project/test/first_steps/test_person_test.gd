# GdUnit generated TestSuite
class_name TestPersonTest
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

# TestSuite generated from
const __source: String = 'res://first_steps/test_person.gd'


func test_full_name() -> void:
	var person = load("res://first_steps/test_person.gd").new("King", "Arthur")
	get_tree().root.add_child(person)
	assert_str(person.full_name()).is_equal("King Arthur")
