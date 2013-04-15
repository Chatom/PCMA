program fonction_carre;

var
	a, b : integer;

function f (c: array[0..50] of integer) : integer;
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
