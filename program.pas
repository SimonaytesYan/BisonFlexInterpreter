begin

    var a;
    var b;
    var c;

    read(a);
    read(b);

    c := a + b * b;

    write(c * c + 1);

    write(10 <= c and c <= 100);
    write(10 <= c or c <= 100);
    write(not c);

end.