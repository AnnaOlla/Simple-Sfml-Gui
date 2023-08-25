# Small-Sfml-Gui
A bunch of simple GUI elements with limited functionality.

It is just a small personal project rather than a good working solution: it was made in order to understand how GUI can work inside.

Thus, it is not deprived of design flaws, maybe, even serious ones. The project is not intended for real use.

## Supported Widgets
* SmallGui::StaticText
  * An aligned text that can be splitted to multiple lines
* SmallGui::TextBox
  * A text input that supports alignment, multiple lines, splitting long words, and deletion of the last symbol
* SmallGui::PushButton
  * A button labeled with text that is supposed to do some action on the click
* SmallGui::DropDownList
  * A list of text-labeled buttons shown and hidden on the click
* SmallGui::CheckBox
  * A widget of binary choice which checkmark is represented by any text supported by font
* SmallGui::IconButton
  * A button labeled with image that is supposed to do some action on the click

## Features
* Consists of a single header and implementation file
* Allows to create one design for multiple elements
* Design is agile and customizable

## Setup
The header and implementation files must be included in the project just like any other code files.

## Definition of Event Handler
The event handler is a mandatory part to process events that the window receives. It finds a desired element and delegates the event to it. Each element knows how to process it. Besides this, the event handler allows to draw all widgets at once (if desired).

Minimal code example:
```c++
#include "SmallSfmlGui.hpp"

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    // Then create an instance of user interface handler
    auto& gui = SmallGui::WidgetPool::getInstance();
    gui.initialize(&window);

    // Run the application
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            // Process events by GUI
            gui.processEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw all non-hidden GUI elements
        window.draw(gui);

        window.display();
    }

    return 0;
}
```

## Definition of Theme
Each widget has its appearance. Themes describe how widgets look like. It is impossible to draw a widget without a theme.

Theme consists of text settings and 3 decoration styles for different states of the widget: idle, hovered and pressed.
* text settings: font, size of characters, horizontal alignment and vertical alignment
* decoration settings: background color, text color, text style, outline thickness, outline color and background texture.

Font is mandatory to draw text. If it is `nullptr`, the text is not drawn.

Background texture is not mandatory and can be `nullptr`. In this case the background is filled with solid color.

Example of creating a theme:
```c++
SmallGui::TextSettings textSettings(font, 36, SmallGui::TextHorizontalAlignment::Center, SmallGui::TextVerticalAlignment::Center);
    
SmallGui::DecorationSettings passiveStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color::Black, texture);
SmallGui::DecorationSettings hoveredStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color::Black, texture);
SmallGui::DecorationSettings pressedStyle(sf::Color::White, sf::Color::Black, sf::Text::Style::Regular, 1.0f, sf::Color::Black, texture);
    
SmallGui::Theme theme(textSettings, passiveStyle, hoveredStyle, pressedStyle);
```

## Definition of Widget
Minimal code example for any widget:
```c++
SmallGui::PushButton button;
button.setTheme(theme);
button.setPosition(sf::Vector2f(50.0f, 50.0f));
button.setSize(sf::Vector2f(200.0f, 100.0f));
```
Each widget has its own features, for example, setting a string as the label.

Widgets can process events. For example, the button can do some action on the mouse click:
```c++
// Using a function
void myFunction()
{
    // some code
}
// ...
button.setAction(sf::Event::MouseButtonReleased, myFunction);
```
```c++
// Using a lambda
button.setAction(sf::Event::MouseButtonReleased, []()
{
    // some code
});
```

## Examples
Some code examples are given in the appropriate directory.

Examples are not provided with the `arial.ttf` file containing the font. Please, make sure you have added it by yourself.

Examples may require the `resources.png` file containing icons. The file has been located in the examples directory.

## License
* The code is free to use and copy
* The resources image has been made by the author and also is completely free
