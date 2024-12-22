begin

    var a;
    var b;
    var c;

    read(a);
    read(b);

    c := a + b * b;

    write(10 <= c and c <= 100);
    write(10 <= c or c <= 100);
    write(not c);

    if c < 10 then
      begin
        write(9);
      end
    else
      begin
        write(11);
      end;

    while c < 10 do
    begin
        write(42);
        c := c + 1;
    end;

    for cnt := 0 to 5 do
    begin
        write(44);
    end;

end.