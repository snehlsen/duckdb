insert into main.orders values ( 40, 2, case when EXISTS ( select ref_0.ps_partkey as c0, ref_0.ps_partkey as c1, ref_0.ps_comment as c2 from main.partsupp as ref_0 where (0) and ((((ref_0.ps_comment is not NULL) or (0)) and (1)) or (((0) or (1)) or (0))) limit 131) then cast(null as VARCHAR) else cast(null as VARCHAR) end , default, default, cast(null as VARCHAR), cast(null as VARCHAR), case when ((70 is not NULL) or (26 is NULL)) or (1) then 81 else 81 end , cast(null as VARCHAR)) 