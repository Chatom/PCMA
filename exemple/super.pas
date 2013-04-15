program fonction_carre;

var
	a, b : integer;

function f () : integer;
begin
   f := c * c;
end;
procedure g (d : integer);
var e : integer;
begin
   g := c div c;
end;

begin
   write(f(2) + g(b));
end.
