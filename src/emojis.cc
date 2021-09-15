#include "emojis.h"

#include "utils.h"

namespace Letters {

#define EMOJI(name) \
	{ #name, Letters::name }
const std::unordered_map<std::string, char32_t> names = {
	EMOJI(green_apple),
	EMOJI(apple),
	EMOJI(pear),
	EMOJI(tangerine),
	EMOJI(lemon),
	EMOJI(banana),
	EMOJI(watermelon),
	EMOJI(grapes),
	EMOJI(blueberries),
	EMOJI(strawberry),
	EMOJI(melon),
	EMOJI(cherries),
	EMOJI(peach),
	EMOJI(mango),
	EMOJI(pineapple),
	EMOJI(coconut),
	EMOJI(kiwi),
	EMOJI(tomato),
	EMOJI(eggplant),
	EMOJI(avocado),
	EMOJI(olive),
	EMOJI(broccoli),
	EMOJI(leafy_green),
	EMOJI(bell_pepper),
	EMOJI(cucumber),
	EMOJI(hot_pepper),
	EMOJI(corn),
	EMOJI(carrot),
	EMOJI(garlic),
	EMOJI(onion),
	EMOJI(potato),
	EMOJI(sweet_potato),
	EMOJI(croissant),
	EMOJI(bagel),
	EMOJI(bread),
	EMOJI(french_bread),
	EMOJI(flatbread),
	EMOJI(pretzel),
	EMOJI(cheese),
	EMOJI(egg),
	EMOJI(cooking),
	EMOJI(butter),
	EMOJI(pancakes),
	EMOJI(waffle),
	EMOJI(bacon),
	EMOJI(cut_of_meat),
	EMOJI(poultry_leg),
	EMOJI(meat_on_bone),
	EMOJI(hotdog),
	EMOJI(hamburger),
	EMOJI(fries),
	EMOJI(pizza),
	EMOJI(sandwich),
	EMOJI(stuffed_flatbread),
	EMOJI(falafel),
	EMOJI(taco),
	EMOJI(burrito),
	EMOJI(tamale),
	EMOJI(salad),
	EMOJI(shallow_pan_of_food),
	EMOJI(fondue),
	EMOJI(canned_food),
	EMOJI(spaghetti),
	EMOJI(ramen),
	EMOJI(stew),
	EMOJI(curry),
	EMOJI(sushi),
	EMOJI(bento),
	EMOJI(dumpling),
	EMOJI(oyster),
	EMOJI(fried_shrimp),
	EMOJI(rice_ball),
	EMOJI(rice),
	EMOJI(rice_cracker),
	EMOJI(fish_cake),
	EMOJI(fortune_cookie),
	EMOJI(moon_cake),
	EMOJI(oden),
	EMOJI(dango),
	EMOJI(shaved_ice),
	EMOJI(ice_cream),
	EMOJI(icecream),
	EMOJI(pie),
	EMOJI(cupcake),
	EMOJI(cake),
	EMOJI(birthday),
	EMOJI(custard),
	EMOJI(lollipop),
	EMOJI(candy),
	EMOJI(chocolate_bar),
	EMOJI(popcorn),
	EMOJI(doughnut),
	EMOJI(cookie),
	EMOJI(chestnut),
	EMOJI(peanuts),
	EMOJI(honey_pot),
	EMOJI(milk),
	EMOJI(baby_bottle),
	EMOJI(coffee),
	EMOJI(tea),
	EMOJI(teapot),
	EMOJI(mate),
	EMOJI(bubble_tea),
	EMOJI(beverage_box),
	EMOJI(cup_with_straw),
	EMOJI(sake),
	EMOJI(beer),
	EMOJI(beers),
	EMOJI(champagne_glass),
	EMOJI(wine_glass),
	EMOJI(tumbler_glass),
	EMOJI(cocktail),
	EMOJI(tropical_drink),
	EMOJI(champagne),
	EMOJI(ice_cube),
	EMOJI(spoon),
	EMOJI(fork_and_knife),
	EMOJI(fork_knife_plate),
	EMOJI(bowl_with_spoon),
	EMOJI(takeout_box),
	EMOJI(chopsticks),
	EMOJI(salt),
};

#undef EMOJI
#define EMOJI(str, name) \
	{ str, Letters::name }
const std::unordered_map<std::string, char32_t> sgtf = {
	EMOJI("gA", green_apple),
	EMOJI("A", apple),
	EMOJI("P", pear),
	EMOJI("TT", tangerine),
	EMOJI("L", lemon),
	EMOJI("B", banana),
	EMOJI("wM", watermelon),
	EMOJI("G", grapes),
	EMOJI("bB", blueberries),
	EMOJI("sB", strawberry),
	EMOJI("M", melon),
	EMOJI("CH", cherries),
	EMOJI("PC", peach),
	EMOJI("MG", mango),
	EMOJI("pA", pineapple),
	EMOJI("cN", coconut),
	EMOJI("K", kiwi),
	EMOJI("T", tomato),
	EMOJI("eP", eggplant),
	EMOJI("AV", avocado),
	EMOJI("O", olive),
	EMOJI("BC", broccoli),
	EMOJI("LT", leafy_green),
	EMOJI("bPP", bell_pepper),
	EMOJI("CC", cucumber),
	EMOJI("hPP", hot_pepper),
	EMOJI("CN", corn),
	EMOJI("C", carrot),
	EMOJI("GA", garlic),
	EMOJI("ZERO", onion),
	EMOJI("PT", potato),
	EMOJI("sPT", sweet_potato),
	EMOJI("cBR", croissant),
	EMOJI("bBR", bagel),
	EMOJI("BR", bread),
	EMOJI("fBR", french_bread),
	EMOJI("flBR", flatbread),
	EMOJI("pBR", pretzel),
	EMOJI("Q", cheese),
	EMOJI("EG", egg),
	EMOJI("?", cooking),
	EMOJI("BU", butter),
	EMOJI("pCK", pancakes),
	EMOJI("W", waffle),
	EMOJI("bM", bacon),
	EMOJI("cM", cut_of_meat),
	EMOJI("pM", poultry_leg),
	EMOJI("mM", meat_on_bone),
	EMOJI("HD", hotdog),
	EMOJI("hFF", hamburger),
	EMOJI("fFF", fries),
	EMOJI("pFF", pizza),
	EMOJI("sFF", sandwich),
	EMOJI("¿", stuffed_flatbread),
	EMOJI("FLF", falafel),
	EMOJI("TK", taco),
	EMOJI("BUR", burrito),
	EMOJI("TM", tamale),
	EMOJI("SL", salad),
	EMOJI("QQ", shallow_pan_of_food),
	EMOJI("FD", fondue),
	EMOJI("CF", canned_food),
	EMOJI("S", spaghetti),
	EMOJI("RA", ramen),
	EMOJI("SW", stew),
	EMOJI("CR", curry),
	EMOJI("suF", sushi),
	EMOJI("BN", bento),
	EMOJI("D", dumpling),
	EMOJI("Y", oyster),
	EMOJI("fsF", fried_shrimp),
	EMOJI("bR", rice_ball),
	EMOJI("R", rice),
	EMOJI("cR", rice_cracker),
	EMOJI("FCK", fish_cake),
	EMOJI("NICE", fortune_cookie),
	EMOJI("mCK", moon_cake),
	EMOJI("oSP", oden),
	EMOJI("dSP", dango),
	EMOJI("sSP", shaved_ice),
	EMOJI("I", ice_cream),
	EMOJI("icSP", icecream),
	EMOJI("pSP", pie),
	EMOJI("cCK", cupcake),
	EMOJI("CK", cake),
	EMOJI("bdCK", birthday),
	EMOJI("cuCK", custard),
	EMOJI("LL", lollipop),
	EMOJI("SP", candy),
	EMOJI("cbSP", chocolate_bar),
	EMOJI("pNHCL", popcorn),
	EMOJI("πR22πR", doughnut),
	EMOJI("BSC", cookie),
	EMOJI("chN", chestnut),
	EMOJI("pN", peanuts),
	EMOJI("CP", honey_pot),
	EMOJI("MLK", milk),
	EMOJI("bMLK", baby_bottle),
	EMOJI("HERESY", coffee),
	EMOJI("YAY", tea),
	EMOJI("HARBOUR", tea),
	EMOJI("pYAY", teapot),
	EMOJI("pHARBOUR", teapot),
	EMOJI("mYAY", mate),
	EMOJI("mHARBOUR", mate),
	EMOJI("bYAY", bubble_tea),
	EMOJI("bHARBOUR", bubble_tea),
	EMOJI("?¿", beverage_box),
	EMOJI("¿?", cup_with_straw),
	EMOJI("sC2H5OH", sake),
	EMOJI("bC2H5OH", beer),
	EMOJI("bbC2H5OH", beers),
	EMOJI("cgC2H5OH", champagne_glass),
	EMOJI("wC2H5OH", wine_glass),
	EMOJI("tC2H5OH", tumbler_glass),
	EMOJI("ckC2H5OH", cocktail),
	EMOJI("trC2H5OH", tropical_drink),
	EMOJI("cC2H5OH", champagne),
	EMOJI("COLD", ice_cube),
	EMOJI("sNOT_FOOD", spoon),
	EMOJI("fkNOT_FOOD", fork_and_knife),
	EMOJI("fkpNOT_FOOD", fork_knife_plate),
	EMOJI("bsNOT_FOOD", bowl_with_spoon),
	EMOJI("tbNOT_FOOD", takeout_box),
	EMOJI("CHOP", chopsticks),
	EMOJI("NACL", salt),
	EMOJI("NHCL", salt),
};

} // namespace Letters