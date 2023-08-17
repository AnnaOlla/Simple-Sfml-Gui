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
  * A button labeled with text that does some action on the click
* SmallGui::DropDownList
  * A list of text-labeled buttons shown and hidden on the click
* SmallGui::CheckBox
  * A widget of binary choice which checkmark is represented by any text supported by font
* SmallGui::IconButton
  * A button labeled with image that does some action on the click

## Features
* Consists of a single header and implementation file
* Allows to create one design for multiple elements
* Design is agile and customizable

## Examples of Use
Minimal code example:
```c++
#include "SmallSfmlGui.hpp"

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");

    // Then create an instance of user interface handler
    auto& ui = SmallGui::WidgetPool::getInstance();
    ui.initialize(&window);

    // Run the application
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            // Process events by GUI
            ui.processEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw all non-hidden GUI elements
        window.draw(ui);

        window.display();
    }

    return 0;
}
```
Also, some code examples are given in the appropriate directory.

Examples are not provided with the `arial.ttf` file containing the font. Please, make sure you have added it by yourself.

Examples may require the `resources.png` file containing icons. The file has been located in the examples directory.

## License
* The code is free to use and copy
* The resources image has been made by the author and also is completely free
