(module
  (type (;0;) (func (result i32)))
  (type (;1;) (func))
  (type (;2;) (func (param i32)))
  (type (;3;) (func (param i32 i32 i32) (result i32)))
  (type (;4;) (func (param i32 i32)))
  (type (;5;) (func (param i32 i32 i32)))
  (type (;6;) (func (param i32) (result i32)))
  (func (;0;) (type 1))
  (func (;1;) (type 0) (result i32)
    (local i32 i32)
    i32.const 1744
    call 2
    i32.const 1792
    i32.const 1824
    call 3
    i32.const 1800
    i32.const 1832
    call 3
    i32.const 0
    local.set 0
    i32.const 0
    i32.const 0
    i32.store8 offset=1840
    block  ;; label = @1
      i32.const 0
      i32.load8_u offset=1824
      i32.const 0
      i32.load8_u offset=1760
      i32.ne
      br_if 0 (;@1;)
      block  ;; label = @2
        loop  ;; label = @3
          local.get 0
          local.tee 1
          i32.const 1
          i32.add
          local.tee 0
          i32.const 16
          i32.eq
          br_if 1 (;@2;)
          local.get 0
          i32.const 1824
          i32.add
          i32.load8_u
          local.get 0
          i32.const 1760
          i32.add
          i32.load8_u
          i32.eq
          br_if 0 (;@3;)
        end
      end
      local.get 1
      i32.const 14
      i32.gt_u
      local.set 0
    end
    local.get 0)
  (func (;2;) (type 2) (param i32)
    (local i32 i32 i32 i32 i32 i32 i32)
    global.get 0
    i32.const 192
    i32.sub
    local.tee 1
    global.set 0
    i32.const 0
    local.set 2
    loop  ;; label = @1
      local.get 2
      i32.const 3
      i32.shl
      local.set 3
      local.get 0
      local.get 2
      i32.add
      i32.load8_u
      local.set 4
      i32.const 0
      local.set 5
      loop  ;; label = @2
        local.get 1
        i32.const 128
        i32.add
        local.get 5
        local.get 3
        i32.add
        i32.add
        local.get 4
        i32.const 7
        local.get 5
        i32.sub
        i32.shr_u
        i32.const 1
        i32.and
        i32.store8
        local.get 5
        i32.const 1
        i32.add
        local.tee 5
        i32.const 8
        i32.ne
        br_if 0 (;@2;)
      end
      local.get 2
      i32.const 1
      i32.add
      local.tee 2
      i32.const 8
      i32.ne
      br_if 0 (;@1;)
    end
    i32.const 0
    local.set 5
    loop  ;; label = @1
      local.get 1
      i32.const 64
      i32.add
      local.get 5
      i32.add
      local.get 5
      i32.const 1104
      i32.add
      i32.load8_s
      local.get 1
      i32.const 128
      i32.add
      i32.add
      i32.const -1
      i32.add
      i32.load8_u
      i32.store8
      local.get 5
      i32.const 1
      i32.add
      local.tee 5
      i32.const 56
      i32.ne
      br_if 0 (;@1;)
    end
    i32.const 0
    local.set 3
    local.get 1
    i32.const 64
    i32.add
    local.set 5
    local.get 1
    local.set 4
    loop  ;; label = @1
      local.get 4
      local.get 5
      local.tee 2
      local.get 3
      i32.const 1216
      i32.add
      i32.load8_s
      local.tee 5
      i32.add
      i32.const 28
      local.get 5
      i32.sub
      local.tee 0
      call 8
      local.tee 4
      i32.const 28
      i32.add
      local.tee 6
      local.get 5
      i32.sub
      local.get 2
      local.get 5
      call 8
      drop
      local.get 6
      local.get 2
      i32.const 28
      i32.add
      local.tee 7
      local.get 5
      i32.add
      local.get 0
      call 8
      drop
      local.get 4
      local.get 5
      i32.sub
      i32.const 56
      i32.add
      local.get 7
      local.get 5
      call 8
      drop
      i32.const 0
      local.set 5
      loop  ;; label = @2
        local.get 3
        i32.const 48
        i32.mul
        local.get 5
        i32.add
        i32.const 1856
        i32.add
        local.get 5
        i32.const 1168
        i32.add
        i32.load8_s
        local.get 4
        i32.add
        i32.const -1
        i32.add
        i32.load8_u
        i32.store8
        local.get 5
        i32.const 1
        i32.add
        local.tee 5
        i32.const 48
        i32.ne
        br_if 0 (;@2;)
      end
      local.get 4
      local.set 5
      local.get 2
      local.set 4
      local.get 3
      i32.const 1
      i32.add
      local.tee 3
      i32.const 16
      i32.ne
      br_if 0 (;@1;)
    end
    local.get 1
    i32.const 192
    i32.add
    global.set 0)
  (func (;3;) (type 4) (param i32 i32)
    (local i32 i32 i32 i32 i32)
    global.get 0
    i32.const 96
    i32.sub
    local.tee 2
    global.set 0
    i32.const 0
    local.set 3
    loop  ;; label = @1
      local.get 3
      i32.const 3
      i32.shl
      local.set 4
      local.get 0
      local.get 3
      i32.add
      i32.load8_u
      local.set 5
      i32.const 0
      local.set 6
      loop  ;; label = @2
        local.get 2
        i32.const 32
        i32.add
        local.get 6
        local.get 4
        i32.add
        i32.add
        local.get 5
        i32.const 7
        local.get 6
        i32.sub
        i32.shr_u
        i32.const 1
        i32.and
        i32.store8
        local.get 6
        i32.const 1
        i32.add
        local.tee 6
        i32.const 8
        i32.ne
        br_if 0 (;@2;)
      end
      local.get 3
      i32.const 1
      i32.add
      local.tee 3
      i32.const 8
      i32.ne
      br_if 0 (;@1;)
    end
    local.get 2
    i32.const 32
    i32.add
    i32.const 32
    i32.add
    local.set 3
    i32.const 0
    local.set 0
    local.get 2
    i32.const 32
    i32.add
    local.set 6
    loop  ;; label = @1
      local.get 6
      local.set 5
      local.get 3
      local.get 2
      local.get 0
      i32.const 48
      i32.mul
      i32.const 1856
      i32.add
      call 4
      i32.const 0
      local.set 6
      loop  ;; label = @2
        local.get 5
        local.get 6
        i32.add
        local.tee 4
        local.get 2
        local.get 6
        i32.add
        i32.load8_u
        local.get 4
        i32.load8_u
        i32.add
        i32.const 1
        i32.and
        i32.store8
        local.get 6
        i32.const 1
        i32.add
        local.tee 6
        i32.const 32
        i32.ne
        br_if 0 (;@2;)
      end
      local.get 3
      local.set 6
      local.get 5
      local.set 3
      local.get 0
      i32.const 1
      i32.add
      local.tee 0
      i32.const 16
      i32.ne
      br_if 0 (;@1;)
    end
    i32.const 0
    local.set 3
    loop  ;; label = @1
      local.get 2
      i32.const 32
      i32.add
      local.get 3
      i32.const 3
      i32.shl
      local.tee 5
      i32.add
      i32.load8_u
      i32.const 1
      i32.and
      local.set 4
      i32.const 1
      local.set 6
      loop  ;; label = @2
        local.get 2
        i32.const 32
        i32.add
        local.get 6
        local.get 5
        i32.add
        i32.add
        i32.load8_u
        i32.const 1
        i32.and
        local.get 4
        i32.const 1
        i32.shl
        i32.or
        local.set 4
        local.get 6
        i32.const 1
        i32.add
        local.tee 6
        i32.const 8
        i32.ne
        br_if 0 (;@2;)
      end
      local.get 1
      local.get 3
      i32.add
      local.get 4
      i32.store8
      local.get 3
      i32.const 1
      i32.add
      local.tee 3
      i32.const 8
      i32.ne
      br_if 0 (;@1;)
    end
    local.get 2
    i32.const 96
    i32.add
    global.set 0)
  (func (;4;) (type 5) (param i32 i32 i32)
    (local i32 i32 i32)
    global.get 0
    i32.const 80
    i32.sub
    local.set 3
    i32.const 0
    local.set 4
    i32.const 0
    local.set 5
    loop  ;; label = @1
      local.get 3
      i32.const 32
      i32.add
      local.get 5
      i32.add
      local.get 5
      i32.const 1024
      i32.add
      i32.load8_s
      local.get 0
      i32.add
      i32.const -1
      i32.add
      i32.load8_u
      i32.store8
      local.get 5
      i32.const 1
      i32.add
      local.tee 5
      i32.const 48
      i32.ne
      br_if 0 (;@1;)
    end
    loop  ;; label = @1
      local.get 3
      i32.const 32
      i32.add
      local.get 4
      i32.add
      local.tee 5
      local.get 2
      local.get 4
      i32.add
      i32.load8_u
      local.get 5
      i32.load8_u
      i32.add
      i32.const 1
      i32.and
      i32.store8
      local.get 4
      i32.const 1
      i32.add
      local.tee 4
      i32.const 48
      i32.ne
      br_if 0 (;@1;)
    end
    i32.const 0
    local.set 0
    i32.const 0
    local.set 4
    loop  ;; label = @1
      local.get 3
      local.get 4
      i32.add
      local.get 0
      i32.const 6
      i32.shl
      local.get 3
      i32.const 32
      i32.add
      local.get 0
      i32.const 6
      i32.mul
      local.tee 2
      i32.add
      local.tee 5
      i32.load8_u
      i32.const 1
      i32.shl
      local.get 5
      i32.const 5
      i32.add
      i32.load8_u
      i32.or
      i32.const 3
      i32.and
      i32.const 4
      i32.shl
      i32.or
      local.get 5
      i32.const 2
      i32.add
      i32.load8_u
      i32.const 2
      i32.shl
      local.get 3
      i32.const 32
      i32.add
      local.get 2
      i32.const 1
      i32.or
      i32.add
      i32.load8_u
      i32.const 3
      i32.shl
      i32.or
      local.get 5
      i32.const 3
      i32.add
      i32.load8_u
      i32.const 1
      i32.shl
      i32.or
      local.get 5
      i32.const 4
      i32.add
      i32.load8_u
      i32.or
      i32.const 15
      i32.and
      i32.or
      i32.const 1232
      i32.add
      i32.load8_u
      local.tee 5
      i32.const 3
      i32.shr_u
      i32.const 1
      i32.and
      i32.store8
      local.get 3
      local.get 4
      i32.const 1
      i32.or
      i32.add
      local.get 5
      i32.const 2
      i32.shr_u
      i32.const 1
      i32.and
      i32.store8
      local.get 3
      local.get 4
      i32.const 2
      i32.or
      i32.add
      local.get 5
      i32.const 1
      i32.shr_u
      i32.const 1
      i32.and
      i32.store8
      local.get 3
      local.get 4
      i32.const 3
      i32.or
      i32.add
      local.get 5
      i32.const 1
      i32.and
      i32.store8
      local.get 4
      i32.const 4
      i32.add
      local.set 4
      local.get 0
      i32.const 1
      i32.add
      local.tee 0
      i32.const 8
      i32.ne
      br_if 0 (;@1;)
    end
    i32.const 0
    local.set 4
    loop  ;; label = @1
      local.get 3
      i32.const 32
      i32.add
      local.get 4
      i32.add
      local.get 4
      i32.const 1072
      i32.add
      i32.load8_s
      local.get 3
      i32.add
      i32.const -1
      i32.add
      i32.load8_u
      i32.store8
      local.get 4
      i32.const 1
      i32.add
      local.tee 4
      i32.const 32
      i32.ne
      br_if 0 (;@1;)
    end
    local.get 1
    local.get 3
    i64.load offset=32
    i64.store align=1
    local.get 1
    i32.const 8
    i32.add
    local.get 3
    i64.load offset=40
    i64.store align=1
    local.get 1
    i32.const 24
    i32.add
    local.get 3
    i32.const 32
    i32.add
    i32.const 24
    i32.add
    i64.load
    i64.store align=1
    local.get 1
    i32.const 16
    i32.add
    local.get 3
    i32.const 32
    i32.add
    i32.const 16
    i32.add
    i64.load
    i64.store align=1)
  (func (;5;) (type 1)
    block  ;; label = @1
      i32.const 1
      i32.eqz
      br_if 0 (;@1;)
      call 0
    end)
  (func (;6;) (type 0) (result i32)
    i32.const 2624)
  (func (;7;) (type 3) (param i32 i32 i32) (result i32)
    (local i32 i32)
    block  ;; label = @1
      local.get 2
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      local.set 3
      loop  ;; label = @2
        local.get 3
        local.get 1
        local.get 2
        i32.const 508
        local.get 2
        i32.const 508
        i32.lt_u
        select
        local.tee 4
        call 8
        local.set 3
        local.get 1
        i32.const 508
        i32.add
        local.set 1
        local.get 3
        i32.const 508
        i32.add
        local.set 3
        local.get 2
        local.get 4
        i32.sub
        local.tee 2
        br_if 0 (;@2;)
      end
    end
    local.get 0)
  (func (;8;) (type 3) (param i32 i32 i32) (result i32)
    (local i32 i32 i32)
    block  ;; label = @1
      local.get 2
      i32.const 512
      i32.lt_u
      br_if 0 (;@1;)
      local.get 0
      local.get 1
      local.get 2
      call 7
      drop
      local.get 0
      return
    end
    local.get 0
    local.get 2
    i32.add
    local.set 3
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        local.get 0
        i32.xor
        i32.const 3
        i32.and
        br_if 0 (;@2;)
        block  ;; label = @3
          block  ;; label = @4
            local.get 0
            i32.const 3
            i32.and
            br_if 0 (;@4;)
            local.get 0
            local.set 2
            br 1 (;@3;)
          end
          block  ;; label = @4
            local.get 2
            i32.const 1
            i32.ge_s
            br_if 0 (;@4;)
            local.get 0
            local.set 2
            br 1 (;@3;)
          end
          local.get 0
          local.set 2
          loop  ;; label = @4
            local.get 2
            local.get 1
            i32.load8_u
            i32.store8
            local.get 1
            i32.const 1
            i32.add
            local.set 1
            local.get 2
            i32.const 1
            i32.add
            local.tee 2
            i32.const 3
            i32.and
            i32.eqz
            br_if 1 (;@3;)
            local.get 2
            local.get 3
            i32.lt_u
            br_if 0 (;@4;)
          end
        end
        block  ;; label = @3
          local.get 3
          i32.const -4
          i32.and
          local.tee 4
          i32.const 64
          i32.lt_u
          br_if 0 (;@3;)
          local.get 2
          local.get 4
          i32.const -64
          i32.add
          local.tee 5
          i32.gt_u
          br_if 0 (;@3;)
          loop  ;; label = @4
            local.get 2
            local.get 1
            i32.load
            i32.store
            local.get 2
            local.get 1
            i32.load offset=4
            i32.store offset=4
            local.get 2
            local.get 1
            i32.load offset=8
            i32.store offset=8
            local.get 2
            local.get 1
            i32.load offset=12
            i32.store offset=12
            local.get 2
            local.get 1
            i32.load offset=16
            i32.store offset=16
            local.get 2
            local.get 1
            i32.load offset=20
            i32.store offset=20
            local.get 2
            local.get 1
            i32.load offset=24
            i32.store offset=24
            local.get 2
            local.get 1
            i32.load offset=28
            i32.store offset=28
            local.get 2
            local.get 1
            i32.load offset=32
            i32.store offset=32
            local.get 2
            local.get 1
            i32.load offset=36
            i32.store offset=36
            local.get 2
            local.get 1
            i32.load offset=40
            i32.store offset=40
            local.get 2
            local.get 1
            i32.load offset=44
            i32.store offset=44
            local.get 2
            local.get 1
            i32.load offset=48
            i32.store offset=48
            local.get 2
            local.get 1
            i32.load offset=52
            i32.store offset=52
            local.get 2
            local.get 1
            i32.load offset=56
            i32.store offset=56
            local.get 2
            local.get 1
            i32.load offset=60
            i32.store offset=60
            local.get 1
            i32.const 64
            i32.add
            local.set 1
            local.get 2
            i32.const 64
            i32.add
            local.tee 2
            local.get 5
            i32.le_u
            br_if 0 (;@4;)
          end
        end
        local.get 2
        local.get 4
        i32.ge_u
        br_if 1 (;@1;)
        loop  ;; label = @3
          local.get 2
          local.get 1
          i32.load
          i32.store
          local.get 1
          i32.const 4
          i32.add
          local.set 1
          local.get 2
          i32.const 4
          i32.add
          local.tee 2
          local.get 4
          i32.lt_u
          br_if 0 (;@3;)
          br 2 (;@1;)
        end
        unreachable
      end
      block  ;; label = @2
        local.get 3
        i32.const 4
        i32.ge_u
        br_if 0 (;@2;)
        local.get 0
        local.set 2
        br 1 (;@1;)
      end
      block  ;; label = @2
        local.get 3
        i32.const -4
        i32.add
        local.tee 4
        local.get 0
        i32.ge_u
        br_if 0 (;@2;)
        local.get 0
        local.set 2
        br 1 (;@1;)
      end
      local.get 0
      local.set 2
      loop  ;; label = @2
        local.get 2
        local.get 1
        i32.load8_u
        i32.store8
        local.get 2
        local.get 1
        i32.load8_u offset=1
        i32.store8 offset=1
        local.get 2
        local.get 1
        i32.load8_u offset=2
        i32.store8 offset=2
        local.get 2
        local.get 1
        i32.load8_u offset=3
        i32.store8 offset=3
        local.get 1
        i32.const 4
        i32.add
        local.set 1
        local.get 2
        i32.const 4
        i32.add
        local.tee 2
        local.get 4
        i32.le_u
        br_if 0 (;@2;)
      end
    end
    block  ;; label = @1
      local.get 2
      local.get 3
      i32.ge_u
      br_if 0 (;@1;)
      loop  ;; label = @2
        local.get 2
        local.get 1
        i32.load8_u
        i32.store8
        local.get 1
        i32.const 1
        i32.add
        local.set 1
        local.get 2
        i32.const 1
        i32.add
        local.tee 2
        local.get 3
        i32.ne
        br_if 0 (;@2;)
      end
    end
    local.get 0)
  (func (;9;) (type 0) (result i32)
    global.get 0)
  (func (;10;) (type 2) (param i32)
    local.get 0
    global.set 0)
  (func (;11;) (type 6) (param i32) (result i32)
    (local i32 i32)
    global.get 0
    local.get 0
    i32.sub
    i32.const -16
    i32.and
    local.tee 1
    global.set 0
    local.get 1)
  (table (;0;) 2 2 funcref)
  (memory (;0;) 256 256)
  (global (;0;) (mut i32) (i32.const 5245520))
  (global (;1;) i32 (i32.const 1792))
  (export "memory" (memory 0))
  (export "check" (func 1))
  (export "inp" (global 1))
  (export "_initialize" (func 5))
  (export "__indirect_function_table" (table 0))
  (export "__errno_location" (func 6))
  (export "stackSave" (func 9))
  (export "stackRestore" (func 10))
  (export "stackAlloc" (func 11))
  (elem (;0;) (i32.const 1) func 0)
  (data (;0;) (i32.const 1024) " \01\02\03\04\05\04\05\06\07\08\09\08\09\0a\0b\0c\0d\0c\0d\0e\0f\10\11\10\11\12\13\14\15\14\15\16\17\18\19\18\19\1a\1b\1c\1d\1c\1d\1e\1f \01\10\07\14\15\1d\0c\1c\11\01\0f\17\1a\05\12\1f\0a\02\08\18\0e \1b\03\09\13\0d\1e\06\16\0b\04\1991)!\19\11\09\01:2*\22\1a\12\0a\02;3+#\1b\13\0b\03<4,$?7/'\1f\17\0f\07>6.&\1e\16\0e\06=5-%\1d\15\0d\05\1c\14\0c\04\00\00\00\00\00\00\00\00\0e\11\0b\18\01\05\03\1c\0f\06\15\0a\17\13\0c\04\1a\08\10\07\1b\14\0d\02)4\1f%/7\1e(3-!0,1'8\225.*2$\1d \01\01\02\02\02\02\02\02\01\02\02\02\02\02\02\01\0e\04\0d\01\02\0f\0b\08\03\0a\06\0c\05\09\00\07\00\0f\07\04\0e\02\0d\01\0a\06\0c\0b\09\05\03\08\04\01\0e\08\0d\06\02\0b\0f\0c\09\07\03\0a\05\00\0f\0c\08\02\04\09\01\07\05\0b\03\0e\0a\00\06\0d\0f\01\08\0e\06\0b\03\04\09\07\02\0d\0c\00\05\0a\03\0d\04\07\0f\02\08\0e\0c\00\01\0a\06\09\0b\05\00\0e\07\0b\0a\04\0d\01\05\08\0c\06\09\03\02\0f\0d\08\0a\01\03\0f\04\02\0b\06\07\0c\00\05\0e\09\0a\00\09\0e\06\03\0f\05\01\0d\0c\07\0b\04\02\08\0d\07\00\09\03\04\06\0a\02\08\05\0e\0c\0b\0f\01\0d\06\04\09\08\0f\03\00\0b\01\02\0c\05\0a\0e\07\01\0a\0d\00\06\09\08\07\04\0f\0e\03\0b\05\02\0c\07\0d\0e\03\00\06\09\0a\01\02\08\05\0b\0c\04\0f\0d\08\0b\05\06\0f\00\03\04\07\02\0c\01\0a\0e\09\0a\06\09\00\0c\0b\07\0d\0f\01\03\0e\05\02\08\04\03\0f\00\06\0a\01\0d\08\09\04\05\0b\0c\07\02\0e\02\0c\04\01\07\0a\0b\06\08\05\03\0f\0d\00\0e\09\0e\0b\02\0c\04\07\0d\01\05\00\0f\0a\03\09\08\06\04\02\01\0b\0a\0d\07\08\0f\09\0c\05\06\03\00\0e\0b\08\0c\07\01\0e\02\0d\06\0f\00\09\0a\04\05\03\0c\01\0a\0f\09\02\06\08\00\0d\03\04\0e\07\05\0b\0a\0f\04\02\07\0c\09\05\06\01\0d\0e\00\0b\03\08\09\0e\0f\05\02\08\0c\03\07\00\04\0a\01\0d\0b\06\04\03\02\0c\09\05\0f\0a\0b\0e\01\07\06\00\08\0d\04\0b\02\0e\0f\00\08\0d\03\0c\09\07\05\0a\06\01\0d\00\0b\07\04\09\01\0a\0e\03\05\0c\02\0f\08\06\01\04\0b\0d\0c\03\07\0e\0a\0f\06\08\00\05\09\02\06\0b\0d\08\01\04\0a\07\09\05\00\0f\0e\02\03\0c\0d\02\08\04\06\0f\0b\01\0a\09\03\0e\05\00\0c\07\01\0f\0d\08\0a\03\07\04\0c\05\06\0b\00\0e\09\02\07\0b\04\01\09\0c\0e\02\00\06\0a\0d\0f\03\05\08\02\01\0e\07\04\0a\08\0d\0f\0c\09\00\03\05\06\0b")
  (data (;1;) (i32.const 1744) "C1nE_KlB\00\00\00\00\00\00\00\00\82\b0\ab\e4-T[+\d7\f2T\b8\1f\1b\c7\92\00\00\00\00"))
