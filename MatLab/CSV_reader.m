START = [2025, 3, 6];

today = datetime("today");
y = year(today);
m = month(today);
d = day(today);

DATA = readtable(sprintf('WINDSDATA_%d_%d_%d.csv', y, m, d), "TextType","string");

DATA = sortrows(DATA,"DATE");

plot(DATA.DATE, DATA.SPEED)