; RUN: opt -disable-output %s -passes="function(basicdel),basicpass" 2>&1 | FileCheck %s

; CHECK: Function: foo
; CHECK-NEXT: 	llvm.dbg.value : 0
; CHECK-NEXT: 	llvm.dbg.declare : 0
; CHECK-NEXT: 	llvm.dbg.label : 0
; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local i32 @foo() local_unnamed_addr #0 !dbg !10 {
entry:
  %a = alloca i32, align 4
  call void @llvm.dbg.declare(metadata ptr %a, metadata !15, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata i32 5, metadata !15, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.value(metadata i32 5, metadata !15, metadata !DIExpression()), !dbg !17
  call void @llvm.dbg.label(metadata !16), !dbg !18
  call void @llvm.dbg.value(metadata i32 6, metadata !15, metadata !DIExpression()), !dbg !17
  ret i32 6, !dbg !19
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.label(metadata) #1



; CHECK: Function: bar
; CHECK-NEXT: 	llvm.dbg.value : 0
; CHECK-NEXT: 	llvm.dbg.declare : 0
; CHECK-NEXT: 	llvm.dbg.label : 0
; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local void @bar() local_unnamed_addr #0 !dbg !20 {
entry:
  call void @llvm.dbg.value(metadata i32 2, metadata !24, metadata !DIExpression()), !dbg !25
  call void @llvm.dbg.value(metadata i32 2, metadata !24, metadata !DIExpression(DW_OP_plus_uconst, 2, DW_OP_stack_value)), !dbg !25
  ret void, !dbg !26
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.value(metadata, metadata, metadata) #2

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C11, file: !1, producer: "clang version 17.0.0 (https://github.com/BgZun/llvm-project.git 0446bfcc5ca206701b511796ed1c8316daa2d169)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "a.c", directory: "/home/bzunic@syrmia.com/Desktop/LLVM", checksumkind: CSK_MD5, checksum: "11c03253b4ce20571a8c9210fd632f77")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 8, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 2}
!8 = !{i32 7, !"debug-info-assignment-tracking", i1 true}
!9 = !{!"clang version 17.0.0 (https://github.com/BgZun/llvm-project.git 0446bfcc5ca206701b511796ed1c8316daa2d169)"}
!10 = distinct !DISubprogram(name: "foo", scope: !1, file: !1, line: 2, type: !11, scopeLine: 2, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !14)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{!15, !16}
!15 = !DILocalVariable(name: "a", scope: !10, file: !1, line: 3, type: !13)
!16 = !DILabel(scope: !10, name: "str", file: !1, line: 4)
!17 = !DILocation(line: 0, scope: !10)
!18 = !DILocation(line: 4, column: 5, scope: !10)
!19 = !DILocation(line: 7, column: 5, scope: !10)
!20 = distinct !DISubprogram(name: "bar", scope: !1, file: !1, line: 11, type: !21, scopeLine: 11, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !23)
!21 = !DISubroutineType(types: !22)
!22 = !{null}
!23 = !{!24}
!24 = !DILocalVariable(name: "b", scope: !20, file: !1, line: 12, type: !13)
!25 = !DILocation(line: 0, scope: !20)
!26 = !DILocation(line: 16, column: 1, scope: !20)
