#pragma once

sf::Font titleFont, headingFont, textFont, detailFont;

bool FontInit() {
    bool loadFontOkay = true;
    if ( !titleFont.loadFromFile("image/fontTitle.ttf") ||
            !headingFont.loadFromFile("image/fontHeading.ttf") ||
            !textFont.loadFromFile("image/fontText.ttf") ||
            !detailFont.loadFromFile("image/fontDetail.ttf") )
    {
        // error
        loadFontOkay = false;
    }
    return loadFontOkay;
}
