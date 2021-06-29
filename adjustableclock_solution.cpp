// Copyright J Carruthers jbc@bu.edu
// solution for adjustable clock
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <string>
#include<thread>


using std::cin;
using std::cout;
using std::string;
using std::to_string;

double besttime() {
  auto now = std::chrono::high_resolution_clock::now();
  auto dur = now.time_since_epoch();
  return dur.count()/1'000'000'000.0;
}

string fill(int t) {
  string s = to_string(t);
  if (s.size() == 1)
     s = '0'+s;
  return s;
}

int main(int argc,char ** argv) {
    double integer_time;
    double time_fract;
    std::time_t adjusted_time;
    string thetime;
    string oldtime;
    string display;

    int adjust=0;

    if (argc>1){
      adjust = std::stoi(argv[1]);
    }    

    sf::RenderWindow window(sf::VideoMode(200, 200), "jbc@bu.edu");
    window.setFramerateLimit(10);
    
    sf::RectangleShape secondhand(sf::Vector2f(3,80));
    sf::RectangleShape minutehand(sf::Vector2f(3,60));
    sf::RectangleShape hourhand(sf::Vector2f(3,40));

    secondhand.setOrigin(1.5,0);
    minutehand.setOrigin(1.5,0);
    hourhand.setOrigin(1.5,0);

    sf::Text words;

    secondhand.setFillColor(sf::Color::Red);
    hourhand.setFillColor(sf::Color::Blue);
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf");
    words.setFont(font);
    words.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                      window.close();
        }

        integer_time = std::time(NULL);

        //time_fract = std::modf(besttime(), &integer_time);
        time_fract = besttime() - integer_time;

        //cerr << integer_time << " " << adjust << " " << time_fract << "\n";

        adjusted_time = integer_time+adjust;

        thetime= std::asctime(std::localtime(&adjusted_time));

        // print out the new time when it changes.
        // since the frame rate is 10, this will
        // always happen in the first 0.1 of the 
        // "new" second

        if ( (oldtime.empty() and time_fract < 0.25) or        // handle first time through
             (not oldtime.empty() and oldtime != thetime) )   // normal operation.
          {
              oldtime=thetime;
              cout << thetime;
        }

        window.clear();
        hourhand.setPosition(100,100);
        minutehand.setPosition(100,100);
        auto orig = minutehand;

        secondhand.setPosition(100,100);

        words.setPosition(10,10);


        std::tm now = *std::localtime(&adjusted_time);

        display = fill(now.tm_hour%12)+
                 ":"+fill(now.tm_min)+
                 ":"+fill(now.tm_sec);


        words.setString(display);

        float frac_secs = now.tm_sec+time_fract;
        float frac_mins = now.tm_min + frac_secs/60;
        float frac_hours = now.tm_hour % 12  + frac_mins / 60;

        secondhand.setRotation(180 + frac_secs*6);
        minutehand.setRotation(180 + frac_mins*6);
        hourhand.setRotation(180 + frac_hours*30);
        window.draw(hourhand);
        window.draw(secondhand);
        window.draw(minutehand);
        window.draw(orig);
        window.draw(words);
        window.display();
    

    }

    return 0;
}