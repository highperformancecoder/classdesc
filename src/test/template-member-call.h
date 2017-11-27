/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

template<class F>
void enumerateRegisterItems(F& f)
{
  f.template registerType<Item>();
  f.template registerType<Port>();
  f.template registerType<Wire>();
  f.template registerType<Operation>();
  f.template registerType<Variable>();
  f.template registerType<Plot>();
  f.template registerType<Group>();
  f.template registerType<Godley>();
}
