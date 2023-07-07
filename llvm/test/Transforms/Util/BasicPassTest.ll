; RUN: opt -disable-output %s -passes="basicpass" 2>&1 | FileCheck %s

; CHECK: Function: foo
; CHECK-NEXT: 	llvm.dbg.value : 3
; CHECK-NEXT: 	llvm.dbg.declare : 1
; CHECK-NEXT: 	llvm.dbg.label : 1
; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local i32 @foo() local_unnamed_addr {
entry:
  %a = alloca i32, align 4
  call void @llvm.dbg.declare(metadata ptr %a, metadata !15, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata i32 5, metadata !15, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata i32 5, metadata !15, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.label(metadata !16), !dbg !17
  call void @llvm.dbg.value(metadata i32 6, metadata !15, metadata !DIExpression()), !dbg !17
  ret i32 6, !dbg !17
}

declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare void @llvm.dbg.label(metadata) #1

; CHECK: Function: bar
; CHECK-NEXT: 	llvm.dbg.value : 2
; CHECK-NEXT: 	llvm.dbg.declare : 0
; CHECK-NEXT: 	llvm.dbg.label : 0
; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local void @bar() local_unnamed_addr #0 !dbg !20 {
entry:
  call void @llvm.dbg.value(metadata i32 2, metadata !24, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata i32 2, metadata !24, metadata !DIExpression(DW_OP_plus_uconst, 2, DW_OP_stack_value)), !dbg !17
  ret void, !dbg !17
}

declare void @llvm.dbg.value(metadata, metadata, metadata) #2

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{ !3}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 17.0.0 (https://github.com/BgZun/llvm-project.git 0446bfcc5ca206701b511796ed1c8316daa2d169)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "a.c", directory: "", checksumkind: CSK_MD5, checksum: "11c03253b4ce20571a8c9210fd632f77")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!11 = !DISubroutineType(types: !12)
!12 = !{}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!15 = !DILocalVariable(name: "a", scope: !20, file: !1, line: 3, type: !13)
!16 = !DILabel(scope: !20, name: "str", file: !1, line: 4)
!17 = !DILocation(line: 0, scope: !20)
!20 = distinct !DISubprogram(name: "bar", scope: !1, file: !1, line: 11, type: !11, scopeLine: 11, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !23)
!23 = !{!24}
!24 = !DILocalVariable(name: "b", scope: !20, file: !1, line: 12, type: !13)
