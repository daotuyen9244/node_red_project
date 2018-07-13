Public Class PLCAddressItemTypeConverter
    Inherits System.ComponentModel.ExpandableObjectConverter
    'Inherits System.ComponentModel.TypeConverter

    Public Overloads Overrides Function CanConvertFrom(ByVal context As System.ComponentModel.ITypeDescriptorContext, ByVal sourceType As System.Type) As Boolean
        If (sourceType.Equals(GetType(String))) Or (sourceType.Equals(GetType(PLCAddressItem))) Then
            Return True
        Else
            Return MyBase.CanConvertFrom(context, sourceType)
        End If
    End Function

    Public Overloads Overrides Function CanConvertTo(ByVal context As System.ComponentModel.ITypeDescriptorContext, ByVal _
                            destinationType As System.Type) As Boolean
        If (destinationType.Equals(GetType(String))) Or (destinationType.Equals(GetType(PLCAddressItem))) Then
            Return True
        Else
            Return MyBase.CanConvertTo(context, destinationType)
        End If
    End Function

    Public Overloads Overrides Function ConvertFrom(ByVal context As System.ComponentModel.ITypeDescriptorContext, ByVal _
        culture As System.Globalization.CultureInfo, ByVal value As Object) As Object
        If (TypeOf value Is String) Then
            Dim txt As String = CType(value, String)
            Dim fields() As String = txt.Split(","c)
            'Dim fields() As String = txt.Split("-")

            Try
                If fields.Length = 1 Then
                    Return New PLCAddressItem(fields(0))
                ElseIf fields.Length > 1 Then
                    Return New PLCAddressItem(fields(0), Convert.ToInt32(fields(1)))
                Else
                    Return New PLCAddressItem()
                End If
            Catch ex As Exception
                Throw New InvalidCastException(value.ToString)
            End Try
        Else
            Return MyBase.ConvertFrom(context, culture, value)
        End If
    End Function

    Public Overloads Overrides Function ConvertTo(ByVal context As System.ComponentModel.ITypeDescriptorContext, ByVal _
        culture As System.Globalization.CultureInfo, ByVal value As Object, ByVal destinationType As System.Type) As Object
        If (destinationType.Equals(GetType(String))) And (TypeOf value Is PLCAddressItem) Then
            Dim mbb As PLCAddressItem = DirectCast(value, PLCAddressItem)
            'Return value.ToString()
            If mbb.PLCAddress IsNot Nothing AndAlso mbb.PLCAddress.Length > 0 Then
                If mbb.NumberOfElements > 1 Then
                    Return mbb.PLCAddress & "," & mbb.NumberOfElements
                Else
                    Return mbb.PLCAddress
                End If
            Else
                Return ""
            End If
        Else
            Return MyBase.ConvertTo(context, culture, value, destinationType)
        End If
    End Function

    Public Overloads Overrides Function GetPropertiesSupported(ByVal context As System.ComponentModel.ITypeDescriptorContext) As Boolean
        Return True
    End Function

    'Public Overloads Overrides Function GetProperties(ByVal context As System.ComponentModel.ITypeDescriptorContext, ByVal value As Object, _
    '                                                  ByVal Attribute() As Attribute) As System.ComponentModel.PropertyDescriptorCollection

    '    Dim properties As System.ComponentModel.PropertyDescriptorCollection = System.ComponentModel.TypeDescriptor.GetProperties(value)
    '    Dim pArray(properties.Count - 3) As System.ComponentModel.PropertyDescriptor
    '    Dim index, index2 As Integer
    '    While index < properties.Count
    '        If properties(index).Name = "LastValue" Or properties(index).Name = "SubscriptionID" Then
    '            'properties.Remove(properties(index))
    '        Else
    '            'NewP.Add(properties(index))
    '            pArray(index2) = properties(index)
    '            index2 += 1
    '        End If
    '        index += 1
    '    End While


    '    Dim NewP As New System.ComponentModel.PropertyDescriptorCollection(pArray)

    '    'Return NewP

    '    Return System.ComponentModel.TypeDescriptor.GetProperties(value)

    '    Return New System.ComponentModel.PropertyDescriptorCollection(System.ComponentModel.TypeDescriptor.GetProperties(value, _
    '                                                    System.ComponentModel.Attributes).Cast(Of System.ComponentModel.PropertyDescriptor)().Where(Function(p) p.Name <> "LastValue" And Function(p) p.Name <> "SubscriptionID").ToArray())

    'End Function

    '* Reference :   http://www.cetix.de/DotNetNews/Thread134029-browsable_attribute_broken_with_custom_TypeConverter.aspx
    Public Overrides Function GetProperties(context As System.ComponentModel.ITypeDescriptorContext, value As Object, attributes As Attribute()) As System.ComponentModel.PropertyDescriptorCollection
        Dim cols As System.ComponentModel.PropertyDescriptorCollection = _
            System.ComponentModel.TypeDescriptor.GetProperties(GetType(PLCAddressItem), attributes)

        Return cols
    End Function


End Class
