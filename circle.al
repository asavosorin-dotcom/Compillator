var xmax = 51;
var ymax = 51;
var r    = 7;

var x = 0;
var y = 0;

var inc = 0;

var adress = 6;

while (y < ymax)
{
    x = 0;
    while (x < xmax)
    {
        if ((x - 20) * (x - 20) + (y - 20)*(y - 20) < r*r)
        {
            draw(adress, 42);
        };

        if ((x - 20) * (x - 20) + (y - 20) * (y - 20) >= r*r)
        {
            draw(adress, 46);
        };

        x = x + 1;
        adress = adress + 1;
    };
    y = y + 1;
};

draw(0, 46);
draw(1, 46);
draw(2, 46);
draw(3, 46);
draw(4, 46);
draw(5, 46);

DRAWram;