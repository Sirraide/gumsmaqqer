#ifndef GUMSMAQQER_EMOJIS_H
#define GUMSMAQQER_EMOJIS_H
#include <string>
#include <unordered_map>
#include <iostream>

namespace Gumsmaq {
enum : wchar_t {
	green_apple			= L'🍏',
	apple				= L'🍎',
	pear				= L'🍐',
	tangerine			= L'🍊',
	lemon				= L'🍋',
	banana				= L'🍌',
	watermelon			= L'🍉',
	grapes				= L'🍇',
	blueberries			= L'🫐',
	strawberry			= L'🍓',
	melon				= L'🍈',
	cherries			= L'🍒',
	peach				= L'🍑',
	mango				= L'🥭',
	pineapple			= L'🍍',
	coconut				= L'🥥',
	kiwi				= L'🥝',
	tomato				= L'🍅',
	eggplant			= L'🍆',
	avocado				= L'🥑',
	olive				= L'🫒',
	broccoli			= L'🥦',
	leafy_green			= L'🥬',
	bell_pepper			= L'🫑',
	cucumber			= L'🥒',
	hot_pepper			= L'🌶',
	corn				= L'🌽',
	carrot				= L'🥕',
	garlic				= L'🧄',
	onion				= L'🧅',
	potato				= L'🥔',
	sweet_potato		= L'🍠',
	croissant			= L'🥐',
	bagel				= L'🥯',
	bread				= L'🍞',
	french_bread		= L'🥖',
	flatbread			= L'🫓',
	pretzel				= L'🥨',
	cheese				= L'🧀',
	egg					= L'🥚',
	cooking				= L'🍳',
	butter				= L'🧈',
	pancakes			= L'🥞',
	waffle				= L'🧇',
	bacon				= L'🥓',
	cut_of_meat			= L'🥩',
	poultry_leg			= L'🍗',
	meat_on_bone		= L'🍖',
	hotdog				= L'🌭',
	hamburger			= L'🍔',
	fries				= L'🍟',
	pizza				= L'🍕',
	sandwich			= L'🥪',
	stuffed_flatbread	= L'🥙',
	falafel				= L'🧆',
	taco				= L'🌮',
	burrito				= L'🌯',
	tamale				= L'🫔',
	salad				= L'🥗',
	shallow_pan_of_food = L'🥘',
	fondue				= L'🫕',
	canned_food			= L'🥫',
	spaghetti			= L'🍝',
	ramen				= L'🍜',
	stew				= L'🍲',
	curry				= L'🍛',
	sushi				= L'🍣',
	bento				= L'🍱',
	dumpling			= L'🥟',
	oyster				= L'🦪',
	fried_shrimp		= L'🍤',
	rice_ball			= L'🍙',
	rice				= L'🍚',
	rice_cracker		= L'🍘',
	fish_cake			= L'🍥',
	fortune_cookie		= L'🥠',
	moon_cake			= L'🥮',
	oden				= L'🍢',
	dango				= L'🍡',
	shaved_ice			= L'🍧',
	ice_cream			= L'🍨',
	icecream			= L'🍦',
	pie					= L'🥧',
	cupcake				= L'🧁',
	cake				= L'🍰',
	birthday			= L'🎂',
	custard				= L'🍮',
	lollipop			= L'🍭',
	candy				= L'🍬',
	chocolate_bar		= L'🍫',
	popcorn				= L'🍿',
	doughnut			= L'🍩',
	cookie				= L'🍪',
	chestnut			= L'🌰',
	peanuts				= L'🥜',
	honey_pot			= L'🍯',
	milk				= L'🥛',
	baby_bottle			= L'🍼',
	coffee				= L'☕',
	tea					= L'🍵',
	teapot				= L'🫖',
	mate				= L'🧉',
	bubble_tea			= L'🧋',
	beverage_box		= L'🧃',
	cup_with_straw		= L'🥤',
	sake				= L'🍶',
	beer				= L'🍺',
	beers				= L'🍻',
	champagne_glass		= L'🥂',
	wine_glass			= L'🍷',
	tumbler_glass		= L'🥃',
	cocktail			= L'🍸',
	tropical_drink		= L'🍹',
	champagne			= L'🍾',
	ice_cube			= L'🧊',
	spoon				= L'🥄',
	fork_and_knife		= L'🍴',
	fork_knife_plate	= L'🍽',
	bowl_with_spoon		= L'🥣',
	takeout_box			= L'🥡',
	chopsticks			= L'🥢',
	salt				= L'🧂',
};

}

extern const std::unordered_map<std::wstring, wchar_t> name_gumsmaq_table;
extern const std::unordered_map<std::wstring, wchar_t> abbr_gumsmaq_table;

#endif // GUMSMAQQER_EMOJIS_H
