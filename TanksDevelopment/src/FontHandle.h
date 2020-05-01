#pragma once

// Font Handling

sf::Font g_TitleFont, g_HeadingFont, g_TextFont, g_DetailFont;

bool FontInit() {
    bool loadFontOkay = true;
    if ( !g_TitleFont.loadFromFile("image/fontTitle.ttf") ||
            !g_HeadingFont.loadFromFile("image/fontHeading.ttf") ||
            !g_TextFont.loadFromFile("image/fontText.ttf") ||
            !g_DetailFont.loadFromFile("image/fontDetail.ttf") )
    {
        // error
        loadFontOkay = false;
    }
    return loadFontOkay;
}
