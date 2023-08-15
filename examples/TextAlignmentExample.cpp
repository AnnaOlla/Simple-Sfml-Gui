#include "SmallSfmlGui.hpp"

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(620, 620), "Text alignment example");
    window.setFramerateLimit(30);

    // Then create an instance of user interface handler
    auto& ui = SmallGui::WidgetPool::getInstance();
    ui.initialize(&window);

    // Create the background
    sf::RectangleShape background;
    background.setFillColor({220, 220, 220});
    background.setPosition({0, 0});
    background.setSize({620, 620});

    // Load a font. Make sure that the font file is put in the project directory (the author does not provide it)
    sf::Font font;
    const auto loadResult = font.loadFromFile("arial.ttf");
    if (!loadResult)
        return 0;

    // Create styles for all possible alignments
    SmallGui::DecorationSettings d;

    SmallGui::TextSettings t1(font, 24, SmallGui::TextHorizontalAlignment::Left, SmallGui::TextVerticalAlignment::Top);
    SmallGui::TextSettings t2(font, 24, SmallGui::TextHorizontalAlignment::Center, SmallGui::TextVerticalAlignment::Top);
    SmallGui::TextSettings t3(font, 24, SmallGui::TextHorizontalAlignment::Right, SmallGui::TextVerticalAlignment::Top);
    SmallGui::TextSettings t4(font, 24, SmallGui::TextHorizontalAlignment::Left, SmallGui::TextVerticalAlignment::Center);
    SmallGui::TextSettings t5(font, 24, SmallGui::TextHorizontalAlignment::Center, SmallGui::TextVerticalAlignment::Center);
    SmallGui::TextSettings t6(font, 24, SmallGui::TextHorizontalAlignment::Right, SmallGui::TextVerticalAlignment::Center);
    SmallGui::TextSettings t7(font, 24, SmallGui::TextHorizontalAlignment::Left, SmallGui::TextVerticalAlignment::Bottom);
    SmallGui::TextSettings t8(font, 24, SmallGui::TextHorizontalAlignment::Center, SmallGui::TextVerticalAlignment::Bottom);
    SmallGui::TextSettings t9(font, 24, SmallGui::TextHorizontalAlignment::Right, SmallGui::TextVerticalAlignment::Bottom);

    SmallGui::Theme theme1(t1, d, d, d);
    SmallGui::Theme theme2(t2, d, d, d);
    SmallGui::Theme theme3(t3, d, d, d);
    SmallGui::Theme theme4(t4, d, d, d);
    SmallGui::Theme theme5(t5, d, d, d);
    SmallGui::Theme theme6(t6, d, d, d);
    SmallGui::Theme theme7(t7, d, d, d);
    SmallGui::Theme theme8(t8, d, d, d);
    SmallGui::Theme theme9(t9, d, d, d);

    // Create elements
    SmallGui::StaticText staticText1;
    staticText1.setPosition({0.0f, 0.0f});
    staticText1.setSize({200.0f, 200.0f});
    staticText1.setTheme(theme1);
    staticText1.setString(L"Lawful Good");

    SmallGui::StaticText staticText2;
    staticText2.setPosition({210.0f, 0.0f});
    staticText2.setSize({200.0f, 200.0f});
    staticText2.setTheme(theme2);
    staticText2.setString(L"Neutral Good");

    SmallGui::StaticText staticText3;
    staticText3.setPosition({420.0f, 0.0f});
    staticText3.setSize({200.0f, 200.0f});
    staticText3.setTheme(theme3);
    staticText3.setString(L"Chaotic Good");

    SmallGui::StaticText staticText4;
    staticText4.setPosition({0.0f, 210.0f});
    staticText4.setSize({200.0f, 200.0f});
    staticText4.setTheme(theme4);
    staticText4.setString(L"Lawful Neutral");

    SmallGui::StaticText staticText5;
    staticText5.setPosition({210.0f, 210.0f});
    staticText5.setSize({200.0f, 200.0f});
    staticText5.setTheme(theme5);
    staticText5.setString(L"True Neutral");

    SmallGui::StaticText staticText6;
    staticText6.setPosition({420.0f, 210.0f});
    staticText6.setSize({200.0f, 200.0f});
    staticText6.setTheme(theme6);
    staticText6.setString(L"Chaotic Neutral");

    SmallGui::StaticText staticText7;
    staticText7.setPosition({0.0f, 420.0f});
    staticText7.setSize({200.0f, 200.0f});
    staticText7.setTheme(theme7);
    staticText7.setString(L"Lawful Evil");

    SmallGui::StaticText staticText8;
    staticText8.setPosition({210.0f, 420.0f});
    staticText8.setSize({200.0f, 200.0f});
    staticText8.setTheme(theme8);
    staticText8.setString(L"Neutral Evil");

    SmallGui::StaticText staticText9;
    staticText9.setPosition({420.0f, 420.0f});
    staticText9.setSize({200.0f, 200.0f});
    staticText9.setTheme(theme9);
    staticText9.setString(L"Chaotic Evil");

    // Run the application
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            ui.processEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.draw(background);
        window.draw(ui);

        window.display();
    }

    return 0;
}
