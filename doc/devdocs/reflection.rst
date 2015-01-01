****************************
Reflection and introspection
****************************

Julia provides several runtime reflection capabilities.

Module bindings
---------------

The exported names for a :obj:`Module` are available using :func:`names(m::Module)`, which will return
an array listing symbols representing each of the exported bindings in the given module.
`names(m::Module, true)` returns symbols for all bindings in ``m``, regardless of export status.

DataType fields
---------------

The names of DataType fields may be interrogated
using :func:`names`. For example, given the following type::

	type Point
	  x::Int
	  y
	end

``names(Point)`` will return the array ``Any[:x, :y]``.

The type of each field in a ``Point`` object is stored in the ``types`` field of the ``Point`` type itself:

	julia> Point.types
	(Int,Any)

While ``x`` is annotated as an ``Int``, ``y`` was unannotated in the type definition, and therefore ``y`` defaults to the ``Any`` type.

Types are themselves represented as a structure called ``DataType``::

	julia> typeof(Point)
	DataType

`names(DataType)` gives the names for each field of `DataType` itself. One of these fields is the ``types`` field observed in the example above.

Subtypes
--------

The *direct* subtypes of any :obj:`DataType` may be listed using
:func:`subtypes`. For example, the abstract :obj:`DataType` :obj:`FloatingPoint`
has four (concrete) subtypes::
	
	julia> subtypes(FloatingPoint)
	4-element Array{Any,1}:
	 BigFloat
	 Float16
	 Float32
	 Float64

Any abstract subtype will also be included in this list, but further subtypes
thereof will not; recursive application of :func:`subtypes` may be used to inspect
the full type tree.

DataType layout
---------------

The internal representation of a DataType is critically important
when interfacing with C code.

:func:`isbits(T::DataType) <isbits>` returns true if `T` is
stored with C-compatible alignment.

:func:`fieldoffsets(T::DataType) <fieldoffsets>` returns the (byte) offset for each
field relative to the start of the type.

Function methods
----------------

The methods of any generic function may be listed using :func:`methods`. The method dispatch
table may be searched for methods accepting a given type using :func:`methodswith`.

Expansion and lowering
----------------------

As discussed in the :ref:`Metaprogramming <man-metaprogramming>` section, the
:func:`macroexpand` function gives the unquoted and interpolated expression (``Expr``) form
for a given macro. To use ``macroexpand``, ``quote`` the expression block itself (otherwise,
the macro will be evaluated and the result will be passed instead!). For example::

   julia> macroexpand( :(@edit println("")) )
   :(Base.edit(println,Base.typesof("")))

The functions :func:`Base.Meta.show_sexpr` and :func:`dump` are used to display S-expr style views
and depth-nested detail views for any expression.

Finally, the :func:`expand` function gives the ``lowered`` form of any expression and is of particular
interest for understanding both macros and top-level statements such as function declarations and
variable assignments::

   julia> expand( :(f() = 1) )
   :($(Expr(:method, :f,
            :((top(tuple))((top(tuple))(),(top(tuple))())),
            AST(:($(Expr(:lambda, Any[], Any[Any[],Any[],Any[]],
                :(begin 
                    return 1
                  end)
            )))),
            false)))

Intermediate and compiled representations
-----------------------------------------

Inspecting the lowered form for functions requires selection of the specific method to display,
because generic functions may have many methods with different type signatures. For this purpose,
method-specific code-lowering is available using :func:`code_lowered(f::Function, (Args...)) <code_lowered>`,
and the type-inferred form is available using :func:`code_typed(f::Function, (Args...)) <code_typed>`.

Closer to the machine, the LLVM intermediate representation of a function may be printed using by
:func:`code_llvm(f::Function, (Args...)) <code_llvm>`, and finally the are available using
:func:`code_native(f::Function, (Args...) <code_native>` (this will trigger JIT compilation/code
generation for any function which has not previously been invoked).

For convenience, there are macro versions of the above functions which take standard function calls
and expand argument types automatically::

 
   julia> @code_llvm +(1,1)

   ; Function Attrs: sspreq
   define i64 @"julia_+_130862"(i64, i64) #0 {
   top:
       %2 = add i64 %1, %0, !dbg !8
       ret i64 %2, !dbg !8
   }

(likewise ``@code_typed``, ``@code_lowered``, and ``@code_native``)
